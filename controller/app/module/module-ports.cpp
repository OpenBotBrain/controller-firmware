#include <module/module-input.hpp>
#include <module/module-output.hpp>
#include <hal-adc.hpp>
#include <hal-gpio.hpp>

static constexpr ModuleInput::Config s_input_config[1] =
{
    {
        .pin1_current_enable_io = INPUT1_PIN1_CURRENT_ENABLE_IO,
        .pin2_detect_io = INPUT1_PIN2_DETECT_IO,
        .pin5_io = INPUT1_PIN5_UTX_PRX_IO,
        .pin6_io = INPUT1_PIN6_URX_PTX_IO,
        .pin6_enable_n_io = INPUT1_PIN6_ENABLE_N_IO,
        .pin1_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT1_PIN1,
        .pin6_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT1_PIN6 ,
        .pin56_mode = {CONFIG_TYPE_I1_INPUT, CONFIG_TYPE_I1_OUTPUT, CONGIG_TYPE_I1_SERIAL, CONFIG_TYPE_I1_I2C},
    }
};

static ModuleInput s_input_port[] = {
    s_input_config[0]
};