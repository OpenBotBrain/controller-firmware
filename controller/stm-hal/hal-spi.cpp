#include <assert.h>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-spi.hpp>

struct SPIData
{
    SPI_HandleTypeDef handler;
    FinishSPICb cb;
    void* param;
};

static const SPIChannelConfig* s_spi_config = nullptr;
static SPIData s_spi_data[SPI_TOTAL];

// ------------------------------------------------------------------------
//                              PUBLIC API
// ------------------------------------------------------------------------
void hal_spi_init_default(const BoardSpecificConfig* board_config)
{
    s_spi_config = board_config->spi_config;
    assert(s_spi_config);

    __HAL_RCC_SPI1_CLK_ENABLE();
}

void hal_spi_init(uint8_t type, FinishSPICb cb, void* param)
{
    assert(type < SPI_TOTAL);

    SPIData* data = &s_spi_data[type];
    SPI_HandleTypeDef* handler = &data->handler;
    const SPIChannelConfig* config = &s_spi_config[type];

    data->cb = cb;
    data->param = param;

    handler->Instance = config->spi;
    handler->Init.Mode = SPI_MODE_MASTER;
    handler->Init.Direction = SPI_DIRECTION_2LINES;
    handler->Init.DataSize = SPI_DATASIZE_8BIT;
    handler->Init.CLKPolarity = SPI_POLARITY_LOW;
    handler->Init.CLKPhase = SPI_PHASE_1EDGE;
    handler->Init.NSS = SPI_NSS_SOFT;
    handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    handler->Init.FirstBit = SPI_FIRSTBIT_MSB;
    handler->Init.TIMode = SPI_TIMODE_DISABLE;
    handler->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handler->Init.CRCPolynomial = 7;
    handler->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    handler->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    assert(HAL_SPI_Init(handler) == HAL_OK);

    __HAL_SPI_ENABLE_IT(handler, SPI_IT_TXE);

    HAL_NVIC_SetPriority(config->irq_type, config->irq_priority, 0);
    HAL_NVIC_EnableIRQ(config->irq_type);
}

bool hal_spi_transmit_receive(uint8_t type, uint8_t* tx_buff, uint8_t* rx_buff, uint32_t size)
{
    assert(type < SPI_TOTAL);
    SPIData& data = s_spi_data[type];

    bool ret = HAL_SPI_TransmitReceive_IT(&data.handler, tx_buff, rx_buff, size) == HAL_OK;

    if (!ret)
    {
        HAL_SPI_Abort_IT(&data.handler);
    }

    return ret;
}

static void s_spi_tx_rx_complete(SPI_HandleTypeDef* hspi)
{
    for (int i = 0; i < SPI_TOTAL; i++)
    {
        SPIData& data = s_spi_data[i];

        if (&data.handler == hspi)
        {
            if (data.cb != nullptr)
            {
                data.cb(data.param);
            }
            return;
        }
    }
}

static SPI_HandleTypeDef* s_get_handler(SPI_TypeDef* spi)
{
    for (int i = 0; i < SPI_TOTAL; i++)
    {
        if (spi == s_spi_data[i].handler.Instance)
        {
            return &s_spi_data[i].handler;
        }
    }
    return nullptr;
}

extern "C"
{

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    s_spi_tx_rx_complete(hspi);
}

void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(s_get_handler(SPI1));
}

void SPI2_IRQHandler(void)
{
    HAL_SPI_IRQHandler(s_get_handler(SPI2));
}


}
