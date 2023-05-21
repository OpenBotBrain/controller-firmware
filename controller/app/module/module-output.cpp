#include <module/module-output.hpp>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-tim.hpp>

void OutputPort::init()
{

}

void OutputPort::update()
{

}


uint16_t OutputPort::get_encoder_ticks()
{
    return 0;
}

float OutputPort::get_voltage(PinID id)
{
    (void) id;
    return 0.0f;
}

void OutputPort::set_mode(Mode mode)
{
    (void) mode;
}

void OutputPort::set_pwm_duty(float duty)
{
    (void) duty;
}