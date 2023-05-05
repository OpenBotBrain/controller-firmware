#include <assert.h>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-spi.hpp>

static const SPIChannelConfig* s_spi_config = nullptr;

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
    (void) type;
    (void) cb;
    (void) param;
}