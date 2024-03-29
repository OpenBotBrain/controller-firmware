#pragma once

#include <device/hardware-device.hpp>

enum class Lego_Device_Type
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

        virtual void init() = 0;

        virtual void update() = 0;
};

Lego_Device_Type get_lego_device_type();
