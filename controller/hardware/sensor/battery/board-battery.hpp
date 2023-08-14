#pragma once

#include <cstdint>
#include <sensor/hardware-sensor.hpp>
#include <system/system-battery.hpp>

static constexpr float BATTERY_VOLTAGE = 3.3f;

class Battery : public Sensor
{
    public:

        Battery() {};

        virtual void init() override;

        virtual void update() override;

        bool connected();

        float voltage();

        float charge();

    private:

        bool m_battery_init = false;

        bool m_battery_connected = false;

        float m_battery_voltage = 0.0f;

        BatteryConversion m_battery_conversion;
};
