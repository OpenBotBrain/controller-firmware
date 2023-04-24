#pragma once

#include <cstdint>

enum UartTypes
{
    UART_TYPE_DEBUG_SERIAL,
    UART_TYPE_TOTAL
};

using FinishCb = void(*)(void*);

void hal_uart_init_default();
void hal_uart_init(const uint8_t type, FinishCb finish_tx_cb, void* param);
void hal_uart_write(const uint8_t type, const uint8_t* data, uint32_t size);
uint32_t hal_uart_read(const uint8_t type, uint8_t* data, uint32_t size);