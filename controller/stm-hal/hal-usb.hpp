#pragma once

using FinishTransmiteCallback = void(*)(void*);

void hal_usb_init();
bool hal_usb_transmitte(const uint8_t* data, uint16_t size, FinishTransmiteCallback cb, void* param);
uint32_t hal_usb_read(uint8_t* data, uint32_t max_data_size);