#include <bitset>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <led/board-neoled.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <gscope/gscope.hpp>
#include <gscope/gscope.hpp>

// IN-PI554FCH - https://www.inolux-corp.com/datasheet/SMDLED/Addressable%20LED/IN-PI554FCH.pdf

// ------------------------------------------------------------------------------------
//                                      SYSTEM API
// ------------------------------------------------------------------------------------

static bool s_sending_data = false;

/**
 * Call back for Neoled timer.
*/
static void s_timer_transfer_finish_callback(void*)
{
    // after this time, timer is ready to send a new color
    s_sending_data = false;
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init Neoled.
 *
 * Calls system_neoled_init()
*/
void NeoLED::init()
{
    uint32_t timer_cnt_frequency = hal_tim_neoled_init(s_timer_transfer_finish_callback,  nullptr);
    m_rgb_timer_on_off_periods[0] = timer_cnt_frequency * 0.0000003f;   // 0
    m_rgb_timer_on_off_periods[1] = timer_cnt_frequency * 0.0000006f;   // 1
}

/**
 * Update Neoled.
 *
 * If the colour has changed then it will load the new RGB values into the LED.
*/
void NeoLED::update()
{
    uint32_t now = hal_timer_32_ms();

    if ((now - m_timestamp) >= 1000)
    {
        m_timestamp = now;
        if (colour_changed())
        {
            m_last_colour = m_rgb_colour;
            load_rgb();
        }
    }
}

/**
 * Turn the RGB LED on or off.
*/
void NeoLED::set_enable(bool enable)
{
    m_rgb_on = enable;
}

/**
 * Set the Brightness value of the RGB LED.
 *
 * @param brightness Neoled_Brightness enum value.
*/
void NeoLED::set_brightness(NeoLED_Brightness brightness)
{
    m_rgb_brightness = brightness;
}

/**
 * Get the Brightness value of the RGB LED.
 *
 * @return Neoled Brightness
*/
NeoLED_Brightness NeoLED::get_brightness(void)
{
    return m_rgb_brightness;
}

/**
 * Set RGB values of LED
 *
 * @param r Red (0 - 255)
 * @param g Green (0 - 255)
 * @param b Blue (0 - 255)
*/
void NeoLED::set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    m_rgb_colour.red = r;
    m_rgb_colour.green = g;
    m_rgb_colour.blue = b;
}

/**
 * Set the Colour value of the RGB LED.
 *
 * @param colour Neoled_Colour struct that colour is being changed to.
*/
void NeoLED::set_colour(NeoLED_Colour colour)
{
    m_rgb_colour = colour;
}

/**
 * Get the current Colour of the RGB LED.
 *
 * @return Neoled Colour
*/
NeoLED_Colour NeoLED::get_colour(void)
{
    return m_rgb_colour;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------

/**
 * Loads the colours into DMA to be transferred to the RGB LED.
*/
void NeoLED::load_rgb()
{
    if (s_sending_data)
    {
        return; // timer is busy sending a new color
    }

    s_sending_data = true;

    std::bitset green = std::bitset<COLOUR_SIZE>((m_rgb_colour.green / m_rgb_brightness) * m_rgb_on);
    std::bitset red   = std::bitset<COLOUR_SIZE>((m_rgb_colour.red   / m_rgb_brightness) * m_rgb_on);
    std::bitset blue  = std::bitset<COLOUR_SIZE>((m_rgb_colour.blue  / m_rgb_brightness) * m_rgb_on);

    for (uint8_t i = 0; i < COLOUR_SIZE; i++)
    {
        m_rgb_timer_data[i] = m_rgb_timer_on_off_periods[green[7 - i]];
        m_rgb_timer_data[i + COLOUR_SIZE] = m_rgb_timer_on_off_periods[red[7 - i]];
        m_rgb_timer_data[i + 2 * COLOUR_SIZE] = m_rgb_timer_on_off_periods[blue[7 - i]];
    }

    hal_timer_neoled_start_dma_transfer(m_rgb_timer_data, (3 * COLOUR_SIZE) + 1);
}

/**
 * Checks to see if the colour has been changed.
 *
 * @return bool
*/
bool NeoLED::colour_changed()
{
    return !(
        m_rgb_colour.red == m_last_colour.red &&
        m_rgb_colour.green == m_last_colour.green &&
        m_rgb_colour.blue == m_last_colour.blue
    );
}
