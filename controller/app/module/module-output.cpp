#include <assert.h>
#include <module/module-output.hpp>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-tim.hpp>

void OutputPort::convert_adc_reading_update()
{
    m_pin5_voltage_v = m_adc_raw_reading_input5 * ADC_TO_VOLTAGE_INPUTS;
    m_pin6_voltage_v = m_adc_raw_reading_input6 * ADC_TO_VOLTAGE_INPUTS;
}

void OutputPort::adc_new_sample_input5(uint16_t value, void* param)
{
    static_cast<OutputPort*>(param)->m_adc_raw_reading_input5 = value;
}

void OutputPort::adc_new_sample_input6(uint16_t value, void* param)
{
    static_cast<OutputPort*>(param)->m_adc_raw_reading_input6 = value;
}

void OutputPort::init()
{
    set_mode(Mode::ANALOG);

    hal_adc_init_channel(m_config.adc_pin5_channel, adc_new_sample_input5, this);
    hal_adc_init_channel(m_config.adc_pin6_channel, adc_new_sample_input6, this);

    hal_tim_encoder_init(m_config.encoder_id);
    hal_tim_pwm_init(m_config.motor_pwm_id);
}

void OutputPort::update()
{
    convert_adc_reading_update();

    if (hal_gpio_read_pin(m_config.fault_io) == false)
    {
        set_pwm_duty(0.0f);
    }
}


uint16_t OutputPort::get_encoder_ticks()
{
    return hal_tim_encoder_get_tick(m_config.encoder_id);
}

float OutputPort::get_voltage(PinID id)
{
    switch (id)
    {
        case PinID::PIN5:
            return m_pin5_voltage_v;
        case PinID::PIN6:
            return m_pin6_voltage_v;
    }

    assert(0);

    return 0.0f;
}

void OutputPort::set_mode(Mode mode)
{
    set_pwm_duty(0.0f);

    switch (mode)
    {
        case Mode::DETECT_PIN:
            // Currently on hardware version A, this mode is not working!!!
            hal_gpio_set_pin(m_config.pin5_adc_enable_n_io);
            hal_gpio_set_pin(m_config.pin6_adc_enable_n_io);
            break;

        case Mode::ENCODER:
        case Mode::ANALOG:
            hal_gpio_reset_pin(m_config.pin5_adc_enable_n_io);
            hal_gpio_reset_pin(m_config.pin6_adc_enable_n_io);
            break;
    }
}

void OutputPort::set_pwm_duty(float duty)
{
    m_pwm_value = duty;
    hal_tim_pwm_set_pwm(m_config.motor_pwm_id, duty);
}

float OutputPort::get_pwm_duty()
{
    return m_pwm_value;
}