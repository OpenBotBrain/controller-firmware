#pragma once

#include <device/hardware-device.hpp>

enum Lego_Device_Type
{
    UNDEFINED_LEGO_DEVICE = 0,
    NXT_I2C_DEVICE,
    NEW_DUMB_DEVICE,
    NEW_UART_DEVICE,

    LEGO_DEVICE_TYPE_TOTAL
};

class LegoDevice
{
    public:

        virtual void init(void) = 0;

        virtual void update(void) = 0;
};

Lego_Device_Type get_lego_device_type();
