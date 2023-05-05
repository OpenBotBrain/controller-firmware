#include <assert.h>
#include <stm-hal/hal-board-init.hpp>

static const SPIChannelConfig* s_spi_config = nullptr;

void hal_spi_init_default(const BoardSpecificConfig* board_config)
{
    s_spi_config = board_config->spi_config;
    assert(s_spi_config);
}