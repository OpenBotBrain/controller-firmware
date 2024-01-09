#include <assert.h>
#include <module/module-common.hpp>
#include <module/module-input.hpp>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-i2c.hpp>
#include <stm-hal/hal-uart.hpp>

void InputPort::convert_adc_reading_update()
{
    ModuleCommon::low_pass_filter_update(m_pin1_voltage_v, m_adc_raw_reading_input1 * ModuleCommon::ADC_TO_VOLTAGE_INPUTS);
    ModuleCommon::low_pass_filter_update(m_pin6_voltage_v, m_adc_raw_reading_input6 * ModuleCommon::ADC_TO_VOLTAGE_INPUTS);
}

void InputPort::uart_tx_end_callback(void* param)
{
    static_cast<InputPort*>(param)->m_write_busy = false;
}

void InputPort::adc_new_sample_input1(uint16_t value, void* param)
{
    static_cast<InputPort*>(param)->m_adc_raw_reading_input1 = value;
}

void InputPort::adc_new_sample_input6(uint16_t value, void* param)
{
    static_cast<InputPort*>(param)->m_adc_raw_reading_input6 = value;
}

void InputPort::init()
{
    set_9v_output(false);
    set_mode(Mode::ANALOG);

    if (m_config.i2c_type != InputPort::INVALID)
    {
        hal_i2c_init(m_config.i2c_type);
    }

    if (m_config.uart_type != InputPort::INVALID)
    {
        hal_uart_init(m_config.uart_type, uart_tx_end_callback, this);
    }

    hal_adc_init_channel(m_config.pin1_adc_channel, adc_new_sample_input1, this);
    hal_adc_init_channel(m_config.pin6_adc_channel, adc_new_sample_input6, this);
}

void InputPort::update()
{
    convert_adc_reading_update();
}

float InputPort::get_voltage_v(PinID id)
{
    switch (id)
    {
        case PinID::PIN1:
            return m_pin1_voltage_v;

        case PinID::PIN6:
            return m_pin6_voltage_v;

        default:
            assert(0);
            break;
    }

    return 0.0;
}

bool InputPort::get_gpio(PinID id)
{
    switch (id)
    {
        case PinID::PIN1:
            return m_adc_raw_reading_input1 > 1024;

        case PinID::PIN2:
            return hal_gpio_read_pin(m_config.pin2_detect_io);

        case PinID::PIN5:
            return hal_gpio_read_pin(m_config.pin5_io);

        case PinID::PIN6:
            return hal_gpio_read_pin(m_config.pin6_io);
    }

    assert(0);

    return false;
}

void InputPort::set_mode(Mode mode)
{
    // as a default, digital switch needs to be disabled!, except analog mode!
    hal_gpio_reset_pin(m_config.pin6_enable_n_io);

    switch (mode)
    {
        case Mode::ANALOG:
            hal_gpio_configure_io(m_config.pin56_mode[Pin56ModeOutput]);
            hal_gpio_reset_pin(m_config.pin5_io);
            hal_gpio_reset_pin(m_config.pin6_io);           // Pin 6 must be low!
            hal_gpio_set_pin(m_config.pin6_enable_n_io);    // Pin 6 must be high when using ADC
            break;

        case Mode::INPUT:
            hal_gpio_configure_io(m_config.pin56_mode[Pin56ModeInput]);
            break;

        case Mode::OUTPUT:
            hal_gpio_configure_io(m_config.pin56_mode[Pin56ModeOutput]);
            hal_gpio_reset_pin(m_config.pin5_io);
            hal_gpio_reset_pin(m_config.pin6_io);
            break;

        case Mode::SERIAL:
            hal_gpio_configure_io(m_config.pin56_mode[Pin56ModeSerial]);
            break;

        case Mode::I2C:
            hal_gpio_configure_io(m_config.pin56_mode[Pin56ModeI2C]);
            break;
    }
}

void InputPort::set_gpio(PinID id, bool enable)
{
    uint16_t io;

    switch (id)
    {
        case PinID::PIN5:
            io = m_config.pin5_io;
            break;

        case PinID::PIN6:
            io = m_config.pin6_io;
            break;

        default:
            assert(0);
            break;
    }

    if (enable)
    {
        hal_gpio_set_pin(io);
    }
    else
    {
        hal_gpio_reset_pin(io);
    }
}

void InputPort::set_9v_output(bool enable)
{
    if (enable)
    {
        hal_gpio_set_pin(m_config.pin1_current_enable_io);
    }
    else
    {
        hal_gpio_reset_pin(m_config.pin1_current_enable_io);
    }
}

bool InputPort::uart_write(const uint8_t* data, uint16_t size)
{
    if (!m_write_busy && m_config.uart_type != INVALID)
    {
        m_write_busy = true;
        hal_uart_write(m_config.uart_type, data, size);
        return true;
    }

    return false;
}

bool InputPort::uart_write_busy()
{
    return m_write_busy;
}

uint16_t InputPort::uart_read(uint8_t* read_buffer, uint16_t buffer_size)
{
    uint16_t read_bytes = 0;

    if (m_config.uart_type != INVALID)
    {
        m_write_busy = true;
        read_bytes = hal_uart_read(m_config.uart_type, read_buffer, buffer_size);
    }

    return read_bytes;
}

void InputPort::i2c_finish(bool error, void* param, uint8_t* data, uint16_t size)
{
    static_cast<InputPort*>(param)->finish_i2c_transaction(error, data, size);
}

void InputPort::finish_i2c_transaction(bool error, uint8_t* data, uint16_t size)
{
    m_i2c_busy = false;
    if (m_r_enable && !error && m_i2c_rx_cb != nullptr)
    {
        m_i2c_rx_cb(m_i2c_param, data, size);
    }
}

bool InputPort::i2c_read(uint8_t address, uint8_t reg, uint16_t data_size, NewI2CData cb, void* param)
{
    if (m_config.i2c_type != INVALID && !m_i2c_busy)
    {
        m_r_enable = true;
        m_i2c_busy = true;
        m_i2c_rx_cb = cb;
        m_i2c_param = param;
        return hal_i2c_read(m_config.i2c_type, address, reg, data_size, i2c_finish, this);
    }

    return false;
}

bool InputPort::i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint16_t buf_size)
{
    if (m_config.i2c_type != INVALID && !m_i2c_busy)
    {
        m_r_enable = false;
        m_i2c_busy = true;
        return hal_i2c_write(m_config.i2c_type, address, reg, buf, buf_size, i2c_finish, this);
    }

    return false;
}
