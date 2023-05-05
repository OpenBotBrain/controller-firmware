#include <config/appconfig.h>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-uart.hpp>
#include <serial_reception/serial_reception.hpp>
#include <optional>

static constexpr uint16_t SERIAL_RX_BUFFER_SIZE = 256;

struct UartData
{
    UART_HandleTypeDef handler;
    DMA_HandleTypeDef tx_dma;
    DMA_HandleTypeDef rx_dma;
    std::optional<SerialReception<SERIAL_RX_BUFFER_SIZE>> rx;
    FinishCb tx_end_cb;
    void* param;
    const UartChannelConfig* config;
};

static const UartChannelConfig* s_uart_config = nullptr;
static UartData s_uart_data[UART_TYPE_TOTAL];

static uint32_t serial_get_rx_couter(void* param)
{
    UartData* serial = static_cast<UartData*>(param);

    if (serial->config->rx_channel == nullptr)
    {
        return serial->rx->get_counter();               // no DMA is in use
    }
    else
    {
        return __HAL_DMA_GET_COUNTER(&serial->rx_dma);  // dma is in use
    }
}

static void serial_start_reception(uint8_t* data, uint32_t size, void* param)
{
    UartData* serial = static_cast<UartData*>(param);

    if (serial->config->rx_channel == nullptr)
    {
        HAL_UART_Receive_IT(&serial->handler, data, 1);        // no DMA is in use
    }
    else
    {
        HAL_UART_Receive_DMA(&serial->handler, data, size);    // dma is in use
    }
}

static constexpr SerialReceptionConfig s_serial_reception_config =
{
    serial_get_rx_couter,
    serial_start_reception,
    false
};

void hal_uart_init_default(const BoardSpecificConfig* board_config)
{
    s_uart_config = board_config->uart_config;
    assert(s_uart_config);

    __HAL_RCC_LPUART1_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();
    __HAL_RCC_UART5_CLK_ENABLE();
}

void hal_uart_init(const uint8_t type, FinishCb finish_tx_cb, void* param)
{
    assert(type < UART_TYPE_TOTAL);

    const UartChannelConfig* config = &s_uart_config[type];
    UartData* serial = &s_uart_data[type];

    // Safe finish transmite callback function and parameter
    serial->tx_end_cb = finish_tx_cb;
    serial->param = param;
    serial->config = config;

    // Configure UART
    UART_HandleTypeDef* uart = &serial->handler;

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
    serial->rx.emplace(s_serial_reception_config, serial);
    serial->rx->init(); // start reading usign DMA or IRQ
}

void hal_uart_write(const uint8_t type, const uint8_t* data, uint32_t size)
{
    assert(type < UART_TYPE_TOTAL);
    HAL_UART_Transmit_DMA(&s_uart_data[type].handler, data, size);
}

uint32_t hal_uart_read(const uint8_t type, uint8_t* data, uint32_t size)
{
    assert(type < UART_TYPE_TOTAL);
    return s_uart_data[type].rx->read(data, size);
}

extern "C" {
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    for (int i = 0; i < UART_TYPE_TOTAL; i++)
    {
        UartData* data = &s_uart_data[i];

        if (huart == &data->handler)
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

        if (huart == &data->handler)
        {
            data->rx->error();
        }
        return;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_TYPE_TOTAL; i++)
    {
        UartData* serial = &s_uart_data[i];

        if (huart == &serial->handler)
        {
            HAL_UART_Receive_IT(huart, serial->rx->next(), 1);
            return;
        }
    }
}

void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_1].tx_dma);    // uart3 tx
}

void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_1].rx_dma);    // uart3 rx
}

void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_4].tx_dma);    // uart1 tx
}

void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_4].rx_dma);    // uart1 rx
}

void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_3].rx_dma);    // uart 2 rx
}

void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_3].tx_dma);    // uart2 tx
}

void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_RPI].tx_dma);        // uart5 tx
}

void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_RPI].rx_dma);        // uart5 rx
}

void DMA2_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_2].tx_dma);    // uart4 tx
}

void DMA2_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_DEBUG_SERIAL].tx_dma);    // lpuart tx
}

void DMA2_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&s_uart_data[UART_TYPE_DEBUG_SERIAL].rx_dma);    // lpuart rx
}

void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&s_uart_data[UART_TYPE_PORT_INPUT_2].handler);
}

}