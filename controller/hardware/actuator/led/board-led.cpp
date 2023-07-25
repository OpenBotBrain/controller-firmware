#include <led/board-led.hpp>
#include <stm-hal/hal-gpio.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init the LEDs.
 *
 * Sets all of the states to false.
*/
void LED::init()
{

}

/**
 * Update the LEDs.
*/
void LED::update()
{
    (m_led_states[0]) ? hal_gpio_set_pin(LED1_IO) : hal_gpio_reset_pin(LED1_IO);
    (m_led_states[1]) ? hal_gpio_set_pin(LED2_IO) : hal_gpio_reset_pin(LED2_IO);
    (m_led_states[2]) ? hal_gpio_set_pin(LED3_IO) : hal_gpio_reset_pin(LED3_IO);
}

/**
 * Reset the LEDs and their states.
*/
void LED::reset()
{
    hal_gpio_reset_pin(LED1_IO);
    hal_gpio_reset_pin(LED2_IO);
    hal_gpio_reset_pin(LED3_IO);

    m_led_states[0] = false;
    m_led_states[1] = false;
    m_led_states[2] = false;
}

/**
 * Set the state of a given LED.
 *
 * @param led_type led type which is having its state changed.
 * @param state state led type is being changed to.
*/
void LED::set_led_state(LED_Type led_type, bool state)
{
    uint8_t led_index = static_cast<uint8_t>(led_type);
    m_led_states[led_index] = state;
}

/**
 * Get the led state of a given led type.
 *
 * @param led_type led which state is being gotten.
 * @return state of led
*/
bool LED::get_led_state(LED_Type led_type)
{
    uint8_t led_index = static_cast<uint8_t>(led_type);
    return m_led_states[led_index];
}

/**
 * Set state of LED 1.
 *
 * @param bool state
*/
void LED::set_led_1(bool state)
{
    m_led_states[0] = state;
}

/**
 * Set state of LED 2.
 *
 * @param bool state
*/
void LED::set_led_2(bool state)
{
    m_led_states[1] = state;
}

/**
 * Set state of LED 3.
 *
 * @param bool state
*/
void LED::set_led_3(bool state)
{
    m_led_states[2] = state;
}
