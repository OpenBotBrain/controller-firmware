#pragma once

#include <cstdint>

class BatteryConversion
{
    public:
        BatteryConversion() {}

        void update_new_value(float voltage_volt)
        {
            float charge = estimate_charge(voltage_volt);
            m_charge_value = m_charge_value * POLE_Z + (1.0f - POLE_Z) * charge;
        }

        float get_charge()
        {
            return m_charge_value;
        }

    private:
        static constexpr float POLE_Z = 0.95f;
        static constexpr uint8_t LOOKUP_TABLE_SIZE = 21;
        static constexpr float s_voltage_to_batterty[LOOKUP_TABLE_SIZE] = {
            3.27f,3.61f,3.69f,3.71f,3.73f,3.75f,3.77f,3.79f,3.80f,3.82f,
            3.84f,3.85f,3.87f,3.91f,3.95f,3.98f,4.02f,4.08f,4.11f,4.15f,
            4.20f};
        static constexpr int s_charge_step_per = 5;
        float m_charge_value {0.0f};

        float estimate_charge(float voltage)
        {
            uint8_t idx = 0;
            if (voltage < s_voltage_to_batterty[0])
            {
                return 0.0f;
            }
            else if (voltage >= s_voltage_to_batterty[LOOKUP_TABLE_SIZE - 1])
            {
                return 100.0f;
            }

            for (uint8_t i = 0; i < LOOKUP_TABLE_SIZE; i++)
            {
                if (voltage >= s_voltage_to_batterty[i])
                {
                    idx = i;
                }
            }

            // run quick interpolation
            uint8_t percentage_start = idx * s_charge_step_per;
            float voltage_start = s_voltage_to_batterty[idx];
            float voltage_stop = s_voltage_to_batterty[idx + 1];

            float x = (float)s_charge_step_per / (voltage_stop - voltage_start);
            return percentage_start + x * (voltage - voltage_start);
        }

};