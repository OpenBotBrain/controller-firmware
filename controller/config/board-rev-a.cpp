#include <config/board-gpio.h>
#include <config/board-rev.hpp>
#include <stm-hal/hal-gpio.hpp>

static constexpr GPIOInitConfig s_gpio_init_rev_a[TOTAL_GPIO] =
{
    //       Name              Port       Pin            Mode             PuPd            AFSel                Speed         initialValue
    { USER_LED1_IO,           GPIOD, GPIO_PIN_7,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0,                   GPIO_SPEED_FREQ_LOW, SET_LOW },
};

static constexpr BoardSpecificConfig s_rev_a_specific_config =
{
    .board_name = "REV A",
};

const GPIOInitConfig* board_rev_a_get_gpio_config()
{
    return s_gpio_init_rev_a;
}

const BoardSpecificConfig* board_rev_a_get_specific_config()
{
    return &s_rev_a_specific_config;
}