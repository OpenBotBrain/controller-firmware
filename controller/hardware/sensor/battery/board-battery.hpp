#pragma once

#include <cstdint>
#include <sensor/hardware-sensor.hpp>

static constexpr float BATTERY_VOLTAGE = 3.3f;

class BatteryConversion
{
    public:

        BatteryConversion() {}

        void update(float voltage);

        float get_charge();

    private:

        float estimate_charge(float voltage);

    private:

        static constexpr float POLE_Z = 0.95f;

        static constexpr uint8_t LOOKUP_TABLE_SIZE = 21;

        static constexpr float s_voltage_to_batterty[LOOKUP_TABLE_SIZE] =
        {
            3.27f, 3.61f, 3.69f, 3.71f, 3.73f, 3.75f, 3.77f, 3.79f,
            3.80f, 3.82f, 3.84f, 3.85f, 3.87f, 3.91f, 3.95f, 3.98f,
            4.02f, 4.08f, 4.11f, 4.15f, 4.20f
        };

        static constexpr int s_charge_step_per = 5;

        float m_battery_charge {0.0f};
};

class Battery : public Sensor
{
    public:

        Battery() {};

        virtual void init() override;

        virtual void update() override;

        bool is_connected();

        float get_voltage();

        float get_charge();

    private:

        bool m_battery_init {false};

        bool m_battery_connected {false};

        float m_battery_voltage {0.0f};

        float m_battery_charge {0.0f};

        BatteryConversion m_battery_conversion;
};
