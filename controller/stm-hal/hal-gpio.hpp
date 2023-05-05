#pragma once

#include <stm32l4xx_hal.h>
#include <stm-hal/hal-board-init.hpp>

enum BoardGPIONames
{
    USER_LED1_IO = 0,

    TOTAL_GPIO
};

void hal_gpio_init_default(const BoardSpecificConfig* board_config);
void hal_gpio_init_initialize(const GPIOInitConfig* gpios_arr, int num_gpios);

bool hal_gpio_read_pin_default(GPIO_TypeDef* port, uint16_t pin);
bool hal_gpio_read_pin(uint16_t pin);
void hal_gpio_reset_pin(uint16_t pin);
void hal_gpio_set_pin(uint16_t pin);
