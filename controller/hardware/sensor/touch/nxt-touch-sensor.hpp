#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>
#include <cstdint>

/**
 * NXT Touch Sensor modes.
 *
 * - NXT_TOUCH_SENSOR_UNDEFINED: undefined mode.
 * - NXT_TOUCH_SENSOR_TOUCH: regular touch mode.
*/
enum class NXT_Touch_Sensor_Mode
{
    UNDEFINED = 0,
    TOUCH
};

class NXTTouchSensor : public LegoSensor
{
    public:

        NXTTouchSensor(InputPort* port);

        NXTTouchSensor(InputPort* port, NXT_Touch_Sensor_Mode sensor_mode);

        virtual void init() override {};

        virtual void update() override {};

        virtual void set_mode(uint8_t sensor_mode) override;

        virtual uint8_t get_mode() override;

        float fetch_sample();

    private:

        InputPort* m_port;

        NXT_Touch_Sensor_Mode m_sensor_mode;
};
