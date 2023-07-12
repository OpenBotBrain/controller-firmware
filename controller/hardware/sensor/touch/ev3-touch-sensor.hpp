#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>
#include <cstdint>

/**
 * EV3 Touch Sensor modes.
 *
 * - EV3_TOUCH_SENSOR_UNDEFINED: undefined mode.
 * - EV3_TOUCH_SENSOR_TOUCH: regular touch mode.
*/
enum EV3_Touch_Sensor_Mode
{
    EV3_TOUCH_SENSOR_UNDEFINED = 0,
    EV3_TOUCH_SENSOR_TOUCH
};

class EV3TouchSensor : public LegoSensor
{
    public:

        EV3TouchSensor(InputPort *port);

        EV3TouchSensor(InputPort *port, EV3_Touch_Sensor_Mode sensor_mode);

        float fetch_sample(void);

        void set_mode(uint8_t sensor_mode);

        uint8_t get_mode(void);

    private:

        InputPort *m_port;

        EV3_Touch_Sensor_Mode m_sensor_mode;
};
