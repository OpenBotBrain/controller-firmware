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
enum class EV3_Touch_Sensor_Mode
{
    UNDEFINED = 0,
    TOUCH
};

class EV3TouchSensor : public LegoSensor
{
    public:

        EV3TouchSensor(InputPort* port);

        EV3TouchSensor(InputPort* port, EV3_Touch_Sensor_Mode sensor_mode);

        virtual void init() override {};

        virtual void update() override {};

        virtual void set_mode(uint8_t sensor_mode) override;

        virtual uint8_t get_mode() override;

        float fetch_sample();

    private:

        InputPort* m_port;

        EV3_Touch_Sensor_Mode m_sensor_mode;
};
