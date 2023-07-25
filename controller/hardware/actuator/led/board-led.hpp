#pragma once

#include <actuator/hardware-actuator.hpp>
#include <cstdint>

static constexpr uint8_t LED_COUNT = 3;

enum class LED_Type
{
    MIDDLE = 0,
    TOP,
    BOTTOM,
};

class LED : public Actuator
{
    public:

        LED() {};

        virtual void init() override;

        virtual void update() override;

        void reset();

        void set_led_state(LED_Type, bool state);

        bool get_led_state(LED_Type);

        void set_led_1(bool state);

        void set_led_2(bool state);

        void set_led_3(bool state);

    private:

        bool m_led_states[LED_COUNT] =
        {
            false, false, false
        };
};