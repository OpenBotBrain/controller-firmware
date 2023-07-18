#pragma once

#include <cstdint>

class Battery
{
    public:

        Battery(void) {};

        void init(void);

        void update(void);

        bool battery_connected(void);

        float battery_voltage(void);

    private:

        bool m_battery_connected;

        float m_battery_voltage;
};
