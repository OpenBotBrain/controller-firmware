#include <app/assert.h>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-clock.hpp>

static const BoardSpecificConfig* s_board_specific_config = nullptr;

void hal_board_init(const BoardSpecificConfig* board_config)
{
    assert(board_config);

    s_board_specific_config = board_config;

    // Reset of all peripherals, Initializes the Flash interface and the Systick.
    HAL_Init();

    // General common periferals
    hal_clock_init_default();
    hal_gpio_init_default(board_config);
    hal_uart_init_default(board_config);
    // hal_tim_init_default(board_config);
    // hal_adc_init_default(board_config);
    // hal_i2c_init_default(board_config);
    // hal_exti_init_default(board_config);
    // hal_spi_init_default(board_config);
}

const BoardSpecificConfig* board_get_specific_configuration()
{
    assert(s_board_specific_config);

    return s_board_specific_config;
}
