#include <config/appconfig.h>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-tim.hpp>

void hal_tim_init_default(uint8_t board_rev)
{
    (void) board_rev;
}

void hal_tim_init(uint8_t type)
{
    (void) type;
}