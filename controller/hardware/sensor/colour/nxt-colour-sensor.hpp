#pragma once

#include <sensor/lego-sensor.hpp>

class NXTColourSensor : public LegoSensor
{
    public:

        NXTColourSensor() {};

        virtual void init() override {};

        virtual void update() override {};

        virtual void set_mode(uint8_t mode) override {};

        virtual uint8_t get_mode() override {};
};
