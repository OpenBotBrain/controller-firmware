#include <module/module-input.hpp>

#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-i2c.hpp>
#include <stm-hal/hal-uart.hpp>

void InputPort::init()
{

}

void InputPort::update()
{

}


float InputPort::get_voltage(PinID id)
{
    (void) id;
    return 0.0;
}

bool InputPort::get_gpio(PinID id)
{
    (void) id;
    return false;
}

void InputPort::set_mode(PinID id, ModeConfiguration mode)
{
    (void) id;
    (void) mode;
}

void InputPort::set_gpio(PinID id, bool enable)
{
    (void) id;
    (void) enable;
}

void InputPort::set_9v_output(bool enable)
{
    (void) enable;
}

void InputPort::uart_write(const uint8_t* data, uint16_t size)
{
    (void) data;
    (void) size;
}

uint16_t InputPort::uart_read(uint8_t* read_buffer, uint16_t buffer_size)
{
    (void) read_buffer;
    (void) buffer_size;
    return 0;
}

bool InputPort::i2c_read(uint8_t address, uint8_t reg, uint16_t data_size)
{
    (void) address;
    (void) reg;
    (void) data_size;
    return false;
}

bool InputPort::i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint16_t buf_size)
{
    (void) address;
    (void) reg;
    (void) buf;
    (void) buf_size;
    return false;
}