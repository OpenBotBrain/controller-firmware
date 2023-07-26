#include <button/board-buttons.hpp>
#include <stm-hal/hal-gpio.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Update the board buttons.
*/
void Buttons::update()
{
    m_button_states[0] = !hal_gpio_read_pin(BUTTON_1_IO);
    m_button_states[1] = !hal_gpio_read_pin(BUTTON_2_IO);
    m_button_states[2] = !hal_gpio_read_pin(BUTTON_3_IO);
}

/**
 * Checks whether the given button is being pressed.
 *
 * @param button_type which button is being pressed?
 * @return is the button pressed?
*/
bool Buttons::is_pressed(Button_Type button_type)
{
    uint8_t button_index = static_cast<uint8_t>(button_type);
    return m_button_states[button_index];
}
