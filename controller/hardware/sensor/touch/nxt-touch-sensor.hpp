#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>
#include <module/module-ports.hpp>
#include <hardware/hardware-manager.hpp>
#include <cstdint>

/**
 * EV3 Touch Sensor modes.
 *
 * - EV3_TOUCH_SENSOR_UNDEFINED: undefined mode.
 * - EV3_TOUCH_SENSOR_TOUCH: regular touch mode.
*/
enum class NXT_Touch_Sensor_Mode
{
    UNDEFINED = 0,
    TOUCH
};

class NXTTouchSensor : public LegoSensor
{
    public:

        NXTTouchSensor(Lego_Sensor_Port sensor_port);

        NXTTouchSensor(Lego_Sensor_Port sensor_port, NXT_Touch_Sensor_Mode sensor_mode);

        virtual void init() override;

        virtual void update() override;

        virtual void set_mode(uint8_t sensor_mode) override;

        virtual uint8_t get_mode() override;

        float fetch_sample();

        bool is_pressed();

    private:

        Lego_Sensor_Port m_sensor_port;

        InputPort* m_input_port;

        InputPort::Config m_port_config;

        NXT_Touch_Sensor_Mode m_sensor_mode;

        bool m_pressed = false;

        float m_voltage = 0.0f;
};
