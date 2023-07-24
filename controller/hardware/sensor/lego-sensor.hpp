#pragma once

#include <module/module-input.hpp>
#include <sensor/hardware-sensor.hpp>
#include <cstdint>

enum class Lego_Sensor_Type
{
    UNDEFINED_LEGO_SENSOR = 0,
    EV3_COLOUR_SENSOR,
    EV3_SOUND_SENSOR,
    EV3_TOUCH_SENSOR,
    EV3_ULTRASONIC_SENSOR,
    NXT_COLOUR_SENSOR,
    NXT_LIGHT_SENSOR,
    NXT_SOUND_SENSOR,
    NXT_TEMP_SENSOR,
    NXT_TOUCH_SENSOR,

    LEGO_SENSOR_TYPE_TOTAL
};

class LegoSensor
{
    public:

        virtual void init() = 0;

        virtual void update() = 0;

        virtual void set_mode(uint8_t mode) = 0;

        virtual uint8_t get_mode() = 0;
};

Lego_Sensor_Type get_lego_sensor_type(InputPort* port);
