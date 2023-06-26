#include <bitset>
#include <string>
#include <system/system-led-rgb.hpp>
#include <stm-hal/hal-gpio.hpp>

/**
 * Abstraction of Set and Reset pin.
 * 
 * @param io IO pin being written to.
 * @param on State of signal to be sent.
*/
static void write_pin(uint16_t io, bool on)
{   
    (on) ? hal_gpio_set_pin(io) : hal_gpio_reset_pin(io);
}

/**
 * Create instance of RGB LED. 
 * Colour defaults to White.
 * On defaults to Off.
 * 
 * @param io Pin of RGB LED 
*/
LEDRGB::LEDRGB(uint16_t io)
{
    // Set IO.
    LEDRGB::m_io = io;
    // Default colour to WHITE.
    LEDRGB::m_colour = WHITE;
    // Set default to OFF.
    LEDRGB::m_on = false;
    // Set default brightness to 5.
    LEDRGB::m_brightness = BRI_5;
}

/**
 * Turn on RGB LED
*/
void LEDRGB::set_on()
{
    LEDRGB::m_on = true;
}

/**
 * Turn off RGB LED
*/
void LEDRGB::set_off()
{
    LEDRGB::m_on = false;
}

/**
 * Set Colour of RGB LED.
 * 
 * @param colour Colour setting of RGB LED.
*/
void LEDRGB::set_colour(Colour colour)
{
    LEDRGB::m_colour = colour;
}

/**
 * Get Colour of RGB LED.
 * 
 * @returns Colour.
*/
Colour LEDRGB::get_colour()
{
    return LEDRGB::m_colour;
}

/**
 * Set Brightness of RGB LED.
 * 
 * @param brightness Brightness setting of RGB LED.
*/
void LEDRGB::set_brightness(Brightness brightness)
{
    LEDRGB::m_brightness = brightness;
}

/**
 * Get Brightness of RGB LED.
 * 
 * @returns Brightness.
*/
Brightness LEDRGB::get_brightness()
{
    return LEDRGB::m_brightness;
}

/**
 * Write a bit to the set Pin.
 * 
 * @param bit Bit being written to Pin.
*/
void LEDRGB::write_io(bool bit)
{
    write_pin(LEDRGB::m_io, bit);
}

/**
 * Show RGB LED
*/
void LEDRGB::show()
{
    // Init pin.
    write_pin(LEDRGB::m_io, 0);

    // When writing to the RGB LED
    //  - Sent in order of Green, Red, Blue.
    //  - MSB sent first.
    //  - Sent in 24 bits.
    if (LEDRGB::m_on)
    {
        std::bitset red =   std::bitset<8>(LEDRGB::m_colour.red);
        std::bitset green = std::bitset<8>(LEDRGB::m_colour.green);
        std::bitset blue =  std::bitset<8>(LEDRGB::m_colour.blue);

        // Green
        for (uint8_t i = 7; i >= 0; i--)
        {
            write_pin(LEDRGB::m_io, (bool)green[i]); 
        }

        // Red
        for (uint8_t i = 7; i >= 0; i--)
        {
            write_pin(LEDRGB::m_io, (bool)red[i]); 
        }

        // Blue
        for (uint8_t i = 7; i >= 0; i--)
        {
            write_pin(LEDRGB::m_io, (bool)blue[i]); 
        }
    }
}