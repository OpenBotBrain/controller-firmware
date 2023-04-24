#pragma once

#include <cstdint>
#include <config/board-gpio.h>
#include <stm32l4xx_hal.h>

typedef enum
{
    SET_LOW = 0,
    SET_HIGH,
    DONT_CARE,
} port_state_t;

struct GPIOInitConfig
{
    uint16_t id;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint16_t mode;
    uint16_t alternate;
    uint16_t pull;
    uint16_t speed;
    port_state_t state;
};

void hal_gpio_init_config(const GPIOInitConfig* gpios, int num_gpios);
void hal_gpio_init_initialize(const GPIOInitConfig* gpios_arr, int num_gpios);

bool hal_gpio_read_pin_default(GPIO_TypeDef* port, uint16_t pin);
bool hal_gpio_read_pin(uint16_t pin);
void hal_gpio_reset_pin(uint16_t pin);
void hal_gpio_set_pin(uint16_t pin);
