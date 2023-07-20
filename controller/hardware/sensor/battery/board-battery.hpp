#pragma once

#include <cstdint>
#include <sensor/hardware-sensor.hpp>

class Battery : public Sensor
{
    public:

        Battery() {};

        virtual void init() override;

        virtual void update() override;

        virtual float fetch_sample() override;

        bool battery_connected();

        float battery_voltage();

    private:

        bool m_battery_connected;

        float m_battery_voltage;
};
