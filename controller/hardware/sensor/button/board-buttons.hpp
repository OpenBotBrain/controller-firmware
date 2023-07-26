#pragma once

#include <sensor/hardware-sensor.hpp>
#include <cstdint>

static constexpr uint8_t BUTTON_COUNT = 3;

enum class Button_Type
{
    MIDDLE = 0,
    BOTTOM,
    TOP,
};

class Buttons : public Sensor
{
    public:

        Buttons() {};

        virtual void init() override {};

        virtual void update() override;

        bool is_pressed(Button_Type button_type);

    private:

        bool m_button_states[BUTTON_COUNT] =
        {
            false, false, false
        };
};
