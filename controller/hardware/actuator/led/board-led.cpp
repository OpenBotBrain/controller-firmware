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
void Led::init()
{
    m_state_led_1 = false;
    m_state_led_2 = false;
    m_state_led_3 = false;
}

/**
 * Update the LEDs.
*/
void Led::update()
{
    (m_state_led_1) ? hal_gpio_set_pin(LED1_IO) : hal_gpio_reset_pin(LED1_IO);
    (m_state_led_2) ? hal_gpio_set_pin(LED2_IO) : hal_gpio_reset_pin(LED2_IO);
    (m_state_led_3) ? hal_gpio_set_pin(LED3_IO) : hal_gpio_reset_pin(LED3_IO);
}

/**
 * Reset the LEDs.
*/
void Led::reset()
{
    hal_gpio_reset_pin(LED1_IO);
    hal_gpio_reset_pin(LED2_IO);
    hal_gpio_reset_pin(LED3_IO);
}

/**
 * Set state of LED 1.
 *
 * @param bool state
*/
void Led::set_led_1(bool state)
{
    m_state_led_1 = state;
}

/**
 * Set state of LED 2.
 *
 * @param bool state
*/
void Led::set_led_2(bool state)
{
    m_state_led_2 = state;
}

/**
 * Set state of LED 3.
 *
 * @param bool state
*/
void Led::set_led_3(bool state)
{
    m_state_led_3 = state;
}
