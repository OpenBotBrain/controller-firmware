#pragma once

#include <actuator/hardware-actuator.hpp>
#include <cstdint>

class Led
{
    public:

        Led(void) {};

        void init(void);

        void update(void);

        void reset(void);

        void set_led_1(bool state);

        void set_led_2(bool state);

        void set_led_3(bool state);

    private:

        bool m_state_led_1;

        bool m_state_led_2;

        bool m_state_led_3;
};