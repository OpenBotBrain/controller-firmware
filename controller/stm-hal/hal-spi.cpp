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
