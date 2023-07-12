#include <led/board-neoled.hpp>
#include <system/system-neoled.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init Neoled.
 *
 * Calls system_neoled_init()
 *
 * defaults:
 * - rgb_on         = false
 * - rgb_colour     = NEO_WHITE
 * - rgb_brightness = NEO_BRI_10
*/
void Neoled::init()
{
    system_neoled_init();

    m_rgb_on = false;
    m_rgb_colour = NEO_WHITE;
    m_rgb_brightness = NEO_BRI_1;
}

/**
 * Update Neoled.
 *
 * If RGB is on then sends set colour, otherwise sends 0, 0, 0.
*/
void Neoled::update()
{
    if (m_rgb_on)
    {
        system_neoled_update
        (
            m_rgb_colour.red / m_rgb_brightness,
            m_rgb_colour.green / m_rgb_brightness,
            m_rgb_colour.blue / m_rgb_brightness
        );
    }
    else
    {
        system_neoled_update(0, 0, 0);
    }
}

/**
 * Turn RGB LED on.
*/
void Neoled::on(void)
{
    m_rgb_on = true;
}

/**
 * Turn RGB LED off.
*/
void Neoled::off(void)
{
    m_rgb_on = false;
}

/**
 * Set the Brightness value of the RGB LED.
 *
 * @param brightness Neoled_Brightness enum value.
*/
void Neoled::set_brightness(Neoled_Brightness brightness)
{
    m_rgb_brightness = brightness;
}

/**
 * Get the Brightness value of the RGB LED.
 *
 * @return Neoled Brightness
*/
Neoled_Brightness Neoled::get_brightness(void)
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
void Neoled::set_rgb(uint8_t r, uint8_t g, uint8_t b)
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
void Neoled::set_colour(Neoled_Colour colour)
{
    m_rgb_colour = colour;
}

/**
 * Get the current Colour of the RGB LED.
 *
 * @return Neoled Colour
*/
Neoled_Colour Neoled::get_colour(void)
{
    return m_rgb_colour;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
