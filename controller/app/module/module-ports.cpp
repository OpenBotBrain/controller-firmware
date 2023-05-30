#include <assert.h>
#include <module/module-ports.hpp>
#include <gscope/gscope.hpp>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-i2c.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-uart.hpp>

static constexpr InputPort::Config s_input_config[TOTAL_INPUT_PORTS] =
{
    {
        .pin1_current_enable_io = INPUT1_PIN1_CURRENT_ENABLE_IO,
        .pin2_detect_io = INPUT1_PIN2_DETECT_IO,
        .pin5_io = INPUT1_PIN5_IO,
        .pin6_io = INPUT1_PIN6_IO,
        .pin6_enable_n_io = INPUT1_PIN6_ENABLE_N_IO,
        .pin1_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT1_PIN1,
        .pin6_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT1_PIN6 ,
        .pin56_mode = {CONFIG_TYPE_I1_INPUT, CONFIG_TYPE_I1_OUTPUT, CONGIG_TYPE_I1_SERIAL, CONFIG_TYPE_I1_I2C},
        .uart_type = UART_TYPE_PORT_INPUT_1,
        .i2c_type = I2C_TYPE_PORT_INPUT_1,
    },
    {
        .pin1_current_enable_io = INPUT2_PIN1_CURRENT_ENABLE_IO,
        .pin2_detect_io = INPUT2_PIN2_DETECT_IO,
        .pin5_io = INPUT2_PIN5_IO,
        .pin6_io = INPUT2_PIN6_IO,
        .pin6_enable_n_io = INPUT2_PIN6_ENABLE_N_IO,
        .pin1_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT2_PIN1,
        .pin6_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT2_PIN6 ,
        .pin56_mode = {CONFIG_TYPE_I2_INPUT, CONFIG_TYPE_I2_OUTPUT, CONGIG_TYPE_I2_SERIAL, CONFIG_TYPE_I2_I2C},
        .uart_type = UART_TYPE_PORT_INPUT_2,
        .i2c_type = I2C_TYPE_PORT_INPUT_2,
    },
    {
        .pin1_current_enable_io = INPUT3_PIN1_CURRENT_ENABLE_IO,
        .pin2_detect_io = INPUT3_PIN2_DETECT_IO,
        .pin5_io = INPUT3_PIN5_IO,
        .pin6_io = INPUT3_PIN6_IO,
        .pin6_enable_n_io = INPUT3_PIN6_ENABLE_N_IO,
        .pin1_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT3_PIN1,
        .pin6_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT3_PIN6 ,
        .pin56_mode = {CONFIG_TYPE_I3_INPUT, CONFIG_TYPE_I3_OUTPUT, CONGIG_TYPE_I3_SERIAL, CONFIG_TYPE_I3_I2C},
        .uart_type = UART_TYPE_PORT_INPUT_3,
        .i2c_type = I2C_TYPE_PORT_INPUT_3,
    },
    {
        .pin1_current_enable_io = INPUT4_PIN1_CURRENT_ENABLE_IO,
        .pin2_detect_io = INPUT4_PIN2_DETECT_IO,
        .pin5_io = INPUT4_PIN5_IO,
        .pin6_io = INPUT4_PIN6_IO,
        .pin6_enable_n_io = INPUT4_PIN6_ENABLE_N_IO,
        .pin1_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT4_PIN1,
        .pin6_adc_channel = ADC_CHANNEL_TYPE_PORT_INPUT4_PIN6 ,
        .pin56_mode = {CONFIG_TYPE_I4_INPUT, CONFIG_TYPE_I4_OUTPUT, CONGIG_TYPE_I4_SERIAL, CONFIG_TYPE_I4_I2C},
        .uart_type = UART_TYPE_PORT_INPUT_4,
        .i2c_type = InputPort::INVALID,
    },
};

static InputPort s_input_port[TOTAL_INPUT_PORTS] =
{
    s_input_config[0],
    s_input_config[1],
    s_input_config[2],
    s_input_config[3]
};

InputPort& module_port_get_input_port(uint8_t input_id)
{
    assert(input_id < TOTAL_INPUT_PORTS);
    return s_input_port[input_id];
}

static constexpr OutputPort::Config s_output_config[TOTAL_OUTPUT_PORTS] =
{
    {
        .pin5_adc_enable_n_io = MOTORA_PIN5_DETECT_IO,
        .pin6_adc_enable_n_io = MOTORA_PIN6_DETECT_IO,
        .encoder_id = TIMER_TYPE_MOTOR_A_ENCODER,
        .motor_pwm_id = TIMER_TYPE_MOTOR_A_PWM,
        .fault_io = MOTORAB_FAULT_N_IO,
        .adc_pin5_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT1_PIN5,
        .adc_pin6_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT1_PIN6,
    },
    {
        .pin5_adc_enable_n_io = MOTORB_PIN5_DETECT_IO,
        .pin6_adc_enable_n_io = MOTORB_PIN6_DETECT_IO,
        .encoder_id = TIMER_TYPE_MOTOR_B_ENCODER,
        .motor_pwm_id = TIMER_TYPE_MOTOR_B_PWM,
        .fault_io = MOTORAB_FAULT_N_IO,
        .adc_pin5_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT2_PIN5,
        .adc_pin6_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT2_PIN6,
    },
    {
        .pin5_adc_enable_n_io = MOTORC_PIN5_DETECT_IO,
        .pin6_adc_enable_n_io = MOTORC_PIN6_DETECT_IO,
        .encoder_id = TIMER_TYPE_MOTOR_C_ENCODER,
        .motor_pwm_id = TIMER_TYPE_MOTOR_C_PWM,
        .fault_io = MOTORCD_FAULT_N_IO,
        .adc_pin5_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT3_PIN5,
        .adc_pin6_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT3_PIN6,
    },
        {
        .pin5_adc_enable_n_io = MOTORD_PIN5_DETECT_IO,
        .pin6_adc_enable_n_io = MOTORD_PIN6_DETECT_IO,
        .encoder_id = TIMER_TYPE_MOTOR_D_ENCODER,
        .motor_pwm_id = TIMER_TYPE_MOTOR_D_PWM,
        .fault_io = MOTORCD_FAULT_N_IO,
        .adc_pin5_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT4_PIN5,
        .adc_pin6_channel = ADC_CHANNEL_TYPE_PORT_OUTPUT4_PIN6,
    }
};

static OutputPort s_output_port[TOTAL_OUTPUT_PORTS] =
{
    s_output_config[0],
    s_output_config[1],
    s_output_config[2],
    s_output_config[3]
};

GScopeChannel(s_voltage_pin1, "input_pin1_voltage", float, TOTAL_INPUT_PORTS)
GScopeChannel(s_voltage_pin6, "input_pin6_voltage", float, TOTAL_INPUT_PORTS)
GScopeChannel(s_state_pin2, "input_pin2_state", uint8_t, TOTAL_INPUT_PORTS)
GScopeChannel(s_state_pin5, "input_pin5_state", uint8_t, TOTAL_INPUT_PORTS)
GScopeChannel(s_state_pin6, "input_pin6_state", uint8_t, TOTAL_INPUT_PORTS)

static void s_debug_update()
{
    static uint32_t s_timestamp;

    uint32_t now = hal_timer_32_ms();

    if ((now - s_timestamp) >= 50)
    {
        s_timestamp = now;

        float pin1_voltage[TOTAL_INPUT_PORTS];
        float pin6_voltage[TOTAL_INPUT_PORTS];
        uint8_t pin2_state[TOTAL_INPUT_PORTS];
        uint8_t pin5_state[TOTAL_INPUT_PORTS];
        uint8_t pin6_state[TOTAL_INPUT_PORTS];


        for (int i = 0; i < TOTAL_INPUT_PORTS; i++)
        {
            pin1_voltage[i] = s_input_port[i].get_voltage_v(InputPort::PinID::PIN1);
            pin2_state[i] = s_input_port[i].get_gpio(InputPort::PinID::PIN2);
            pin5_state[i] = s_input_port[i].get_gpio(InputPort::PinID::PIN5);
            pin6_state[i] = s_input_port[i].get_gpio(InputPort::PinID::PIN6);
            pin6_voltage[i] = s_input_port[i].get_voltage_v(InputPort::PinID::PIN6);
        }

        s_voltage_pin1.produce(pin1_voltage);
        s_voltage_pin6.produce(pin6_voltage);
        s_state_pin2.produce(pin2_state);
        s_state_pin5.produce(pin5_state);
        s_state_pin6.produce(pin6_state);
    }
}

OutputPort& module_port_get_outpur_port(uint8_t output_id)
{
    assert(output_id < TOTAL_OUTPUT_PORTS);
    return s_output_port[output_id];
}

void module_port_init()
{
    for (int i = 0; i < TOTAL_INPUT_PORTS; i++)
    {
        s_input_port[i].init();
    }

    for (int i = 0; i < TOTAL_OUTPUT_PORTS; i++)
    {
        s_output_port[i].init();
    }
}

void module_port_update()
{
    for (int i = 0; i < TOTAL_INPUT_PORTS; i++)
    {
        s_input_port[i].update();
    }

    for (int i = 0; i < TOTAL_OUTPUT_PORTS; i++)
    {
        s_output_port[i].update();
    }

    s_debug_update();
}

// Debug input ports
static void s_enable_9volt_output(int idx, int enable)
{
    if (idx >= 0 && idx < TOTAL_INPUT_PORTS)
    {
        s_input_port[idx].set_9v_output(enable != 0);
        GSDebug("Input port %d - 9 Volt out is now %s", idx, (enable != 0) ? "ON" : "OFF");
    }
}
GScopeCommand("input_9v_enable", s_enable_9volt_output)

static void s_set_pin_mode(int idx, int mode)
{
    if (idx >= 0 && idx < TOTAL_INPUT_PORTS &&
        mode >= 0 && mode < 5)
    {
        s_input_port[idx].set_mode(static_cast<InputPort::ModeConfiguration>(mode));
        GSDebug("Input port %d - Mode is %d", idx, mode);
    }
}
GScopeCommand("input_mode", s_set_pin_mode)

static void s_input_mode_type()
{
    GSDebug("[0] Analog");
    GSDebug("[1] Input");
    GSDebug("[2] Output");
    GSDebug("[3] Serial");
    GSDebug("[4] I2C");
}
GScopeCommand("input_mode?", s_input_mode_type)

static void s_set_pin_enable(int idx, int pin, int enable)
{
    if (idx >= 0 && idx < TOTAL_INPUT_PORTS &&
        pin >= 5 && pin <= 6)
    {
        InputPort::PinID pinid = pin == 5 ? InputPort::PinID::PIN5 : InputPort::PinID::PIN6;
        s_input_port[idx].set_mode(InputPort::ModeConfiguration::OUTPUT);
        s_input_port[idx].set_gpio(pinid, enable != 0);
        GSDebug("Input port %d - Mode is %d", idx, enable);
    }
}
GScopeCommand("input_set", s_set_pin_enable)

static void s_test_serial(int idx)
{
    static char s_test_msg[] = "Hello, this is a test message";
    if (idx >= 0 && idx < TOTAL_INPUT_PORTS)
    {
        s_input_port[idx].set_mode(InputPort::ModeConfiguration::SERIAL);
        s_input_port[idx].uart_write((const uint8_t*)s_test_msg, sizeof(s_test_msg));
        GSDebug("Test OK");
    }
}
GScopeCommand("input_serial_test", s_test_serial)