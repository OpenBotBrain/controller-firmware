#pragma once

#include <stm32l4xx_hal.h>
#include <stm-hal/hal-board-init.hpp>

enum UartTypes
{
    UART_TYPE_DEBUG_SERIAL = 0,
    UART_TYPE_PORT_INPUT_1,
    UART_TYPE_PORT_INPUT_2,
    UART_TYPE_PORT_INPUT_3,
    UART_TYPE_PORT_INPUT_4,
    UART_TYPE_PORT_RPI,
    UART_TYPE_TOTAL
};

using FinishCb = void(*)(void*);

void hal_uart_init_default(const BoardSpecificConfig* board_config);
void hal_uart_init(const uint8_t type, FinishCb finish_tx_cb, void* param);
void hal_uart_write(const uint8_t type, const uint8_t* data, uint32_t size);
uint32_t hal_uart_read(const uint8_t type, uint8_t* data, uint32_t size);