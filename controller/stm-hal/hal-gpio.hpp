#pragma once

#include <cstdint>

enum BoardGPIONames
{
    USER_LED1_IO = 0,

    TOTAL_GPIO
};

bool hal_gpio_read_pin(uint16_t io);
void hal_gpio_reset_pin(uint16_t io);
void hal_gpio_set_pin(uint16_t io);
uint16_t hal_gpio_get_pin(uint16_t io);
