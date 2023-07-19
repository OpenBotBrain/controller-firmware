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
enum NXT_Touch_Sensor_Mode
{
    NXT_TOUCH_SENSOR_UNDEFINED = 0,
    NXT_TOUCH_SENSOR_TOUCH
};

class NXTTouchSensor : public LegoSensor
{
    public:

        NXTTouchSensor(InputPort* port);

        NXTTouchSensor(InputPort* port, NXT_Touch_Sensor_Mode sensor_mode);

        float fetch_sample(void);

        void set_mode(uint8_t sensor_mode);

        uint8_t get_mode(void);

    private:

        InputPort* m_port;

        NXT_Touch_Sensor_Mode m_sensor_mode;
};
