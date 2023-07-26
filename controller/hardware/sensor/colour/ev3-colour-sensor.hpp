#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>
#include <cstdint>

enum EV3_Colour_Sensor_Mode
{
    EV3_COLOUR_SENSOR_UNDEFINED = 0,
    EV3_COLOUR_SENSOR_RED,
    EV3_COLOUR_SENSOR_GREEN,
    EV3_COLOUR_SENSOR_BLUE,
    EV3_COLOUR_SENSOR_WHITE,
    EV3_COLOUR_SENSOR_RGB
};

class EV3ColourSensor : public LegoSensor
{
    public:

        EV3ColourSensor(InputPort* port);

        EV3ColourSensor(InputPort* port, EV3_Colour_Sensor_Mode sensor_mode);

        float fetch_sample();

        virtual void init() override {};

        virtual void update() override {};

        virtual void set_mode(uint8_t mode) override;

        virtual uint8_t get_mode() override;

    private:

        InputPort* m_port;

        EV3_Colour_Sensor_Mode m_sensor_mode;
};
