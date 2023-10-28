#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>

class NXTLightSensor : public LegoSensor
{
    public:

        NXTLightSensor() {};

        virtual void init() override = 0;

        virtual void update() override = 0;

        virtual void set_mode(uint8_t sensor_mode) override = 0;

        virtual uint8_t get_mode() override = 0;

    private:

        InputPort* m_port;
};
