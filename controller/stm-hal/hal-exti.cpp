#include <assert.h>
#include <stm-hal/hal-board-init.hpp>

static const EXTIChannelConfig* s_exti_config = nullptr;

void hal_exti_init_default(const BoardSpecificConfig* board_config)
{
    s_exti_config = board_config->exti_config;
    assert(s_exti_config);
}