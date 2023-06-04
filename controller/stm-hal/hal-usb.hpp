#pragma once

enum USBTYPES : uint8_t
{
    USB_TYPE_GSCOPE = 0,

    USB_CALLBACK_TYPES
};

using USBDataCb = bool(*)(const uint8_t* data, uint16_t size, void* param);
using FinishTransmiteCallback = void(*)(void*);

void hal_usb_init();
bool hal_usb_transmitte(const uint8_t* data, uint16_t size, FinishTransmiteCallback cb, void* param);
bool hal_usb_reception_add_handler(uint8_t id, USBDataCb cb, void* param);