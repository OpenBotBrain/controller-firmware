#include <config/appconfig.h>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-uart.hpp>
#include <serial_reception/serial_reception.hpp>
#include <optional>

static constexpr uint16_t SERIAL_RX_BUFFER_SIZE = 256;

struct UartConfig
{
    USART_TypeDef* uart;
    uint32_t speed;
    DMA_Channel_TypeDef* tx_channel;
    uint8_t tx_request;
    IRQn_Type tx_irq_type;
    DMA_Channel_TypeDef* rx_channel;
    uint8_t rx_request;
    IRQn_Type rx_irq_type;
    uint8_t irq_priority;
};

// STM32L496ZGTx
static constexpr UartConfig s_uart_config[UART_TYPE_TOTAL] =
{
    { LPUART1, 921600, DMA2_Channel6, DMA_REQUEST_4, DMA2_Channel6_IRQn, DMA2_Channel7, DMA_REQUEST_4, DMA2_Channel7_IRQn, PRI_HARD_LPUART1   }, // UART_TYPE_DEBUG_SERIAL
    { USART3,  115200, DMA1_Channel2, DMA_REQUEST_2, DMA1_Channel2_IRQn, DMA1_Channel3, DMA_REQUEST_2, DMA1_Channel3_IRQn, PRI_HARD_PORT_UART }, // UART_TYPE_PORT_INPUT_1
    { UART4,   115200, DMA2_Channel3, DMA_REQUEST_2, DMA2_Channel3_IRQn, nullptr,       0,             UART4_IRQn,         PRI_HARD_PORT_UART }, // UART_TYPE_PORT_INPUT_2
    { USART2,  115200, DMA1_Channel7, DMA_REQUEST_2, DMA1_Channel7_IRQn, DMA1_Channel6, DMA_REQUEST_2, DMA1_Channel6_IRQn, PRI_HARD_PORT_UART }, // UART_TYPE_PORT_INPUT_3
    { USART1,  115200, DMA1_Channel4, DMA_REQUEST_2, DMA1_Channel4_IRQn, DMA1_Channel5, DMA_REQUEST_2, DMA1_Channel5_IRQn, PRI_HARD_PORT_UART }, // UART_TYPE_PORT_INPUT_4
    { UART5,   921600, DMA2_Channel1, DMA_REQUEST_2, DMA2_Channel1_IRQn, DMA2_Channel2, DMA_REQUEST_2, DMA2_Channel2_IRQn, PRI_HARD_RPI_UART  }, // UART_TYPE_PORT_RPI
};

struct UartData
{
    UART_HandleTypeDef uart;
    DMA_HandleTypeDef tx_dma;
    DMA_HandleTypeDef rx_dma;
    std::optional<SerialReception<SERIAL_RX_BUFFER_SIZE>> rx;
    FinishCb tx_end_cb;
    void* param;
};

static UartData s_uart_data[UART_TYPE_TOTAL];

static uint32_t get_dma_couter_debug_uart(void* param)
{
    UART_HandleTypeDef* uart = static_cast<UART_HandleTypeDef*>(param);
    return __HAL_DMA_GET_COUNTER(uart->hdmarx);

    // TODO: SPECIAL CASE UART 4
}

static void dma_read_debug_uart(uint8_t* data, uint32_t size, void* param)
{
    UART_HandleTypeDef* uart = static_cast<UART_HandleTypeDef*>(param);
    HAL_UART_Receive_DMA(uart, data, size);

    // TODO: SPECIAL CASE UART 4
}

static constexpr SerialReceptionConfig s_serial_reception_config =
{
    get_dma_couter_debug_uart,
    dma_read_debug_uart,
    false
};

void hal_uart_init_default(uint8_t board_rev)
{
    (void) board_rev;

    __HAL_RCC_LPUART1_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
}

void hal_uart_init(const uint8_t type, FinishCb finish_tx_cb, void* param)
{
    assert(type < UART_TYPE_TOTAL);

    const UartConfig* config = &s_uart_config[type];
    UartData* serial = &s_uart_data[type];

    // Safe finish transmite callback function and parameter
    serial->tx_end_cb = finish_tx_cb;
    serial->param = param;

    // Configure UART
    UART_HandleTypeDef* uart = &serial->uart;

    uart->Instance = config->uart;
    uart->Init.BaudRate = config->speed;
    uart->Init.WordLength = UART_WORDLENGTH_8B;
    uart->Init.StopBits = UART_STOPBITS_1;
    uart->Init.Parity = UART_PARITY_NONE;
    uart->Init.Mode = UART_MODE_TX_RX;
    uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart->Init.OverSampling = UART_OVERSAMPLING_16;
    uart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    uart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    assert(HAL_UART_Init(uart) == HAL_OK);

    // Configure TX DMA
    if (config->tx_channel != nullptr)
    {
        DMA_HandleTypeDef* tx_dma = &s_uart_data[type].tx_dma;

        tx_dma->Instance = config->tx_channel;
        tx_dma->Init.Request = config->tx_request;
        tx_dma->Init.Direction = DMA_MEMORY_TO_PERIPH;
        tx_dma->Init.PeriphInc = DMA_PINC_DISABLE;
        tx_dma->Init.MemInc = DMA_MINC_ENABLE;
        tx_dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        tx_dma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        tx_dma->Init.Mode = DMA_NORMAL;
        tx_dma->Init.Priority = DMA_PRIORITY_LOW;
        assert(HAL_DMA_Init(tx_dma) == HAL_OK);

        __HAL_LINKDMA(uart, hdmatx, *tx_dma);

        __HAL_DMA_ENABLE_IT(tx_dma, DMA_IT_TC);

    }

    HAL_NVIC_SetPriority(config->tx_irq_type, config->irq_priority, 0);
    HAL_NVIC_EnableIRQ(config->tx_irq_type);

    // Configure RX DMA
    if (config->rx_channel != nullptr)
    {
        DMA_HandleTypeDef* rx_dma = &s_uart_data[type].rx_dma;

        rx_dma->Instance = config->rx_channel;
        rx_dma->Init.Request = config->rx_request;;
        rx_dma->Init.Direction = DMA_PERIPH_TO_MEMORY;
        rx_dma->Init.PeriphInc = DMA_PINC_DISABLE;
        rx_dma->Init.MemInc = DMA_MINC_ENABLE;
        rx_dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        rx_dma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        rx_dma->Init.Mode = DMA_CIRCULAR;
        rx_dma->Init.Priority = DMA_PRIORITY_LOW;
        assert(HAL_DMA_Init(rx_dma) == HAL_OK);

        __HAL_LINKDMA(uart, hdmarx, *rx_dma);
    }

    HAL_NVIC_SetPriority(config->rx_irq_type, config->irq_priority, 0);
    HAL_NVIC_EnableIRQ(config->rx_irq_type);

    // Call object constructor
    serial->rx.emplace(s_serial_reception_config, uart);
    serial->rx->init();
}

void hal_uart_write(const uint8_t type, const uint8_t* data, uint32_t size)
{
    assert(type >= 0 && type < UART_TYPE_TOTAL);
    HAL_UART_Transmit_DMA(&s_uart_data[type].uart, data, size);
}

uint32_t hal_uart_read(const uint8_t type, uint8_t* data, uint32_t size)
{
    assert(type >= 0 && type < UART_TYPE_TOTAL);
    return s_uart_data[type].rx->read(data, size);
}

extern "C" {
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    for (int i = 0; i < UART_TYPE_TOTAL; i++)
    {
        UartData* data = &s_uart_data[i];

        if (huart == &data->uart)
        {
            if (data->tx_end_cb != nullptr)
            {
                data->tx_end_cb(data->param);
            }
        }
        return;
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
    for (int i = 0; i < UART_TYPE_TOTAL; i++)
    {
        UartData* data = &s_uart_data[i];

        if (huart == &data->uart)
        {
            data->rx->error();
        }
        return;
    }
}

}