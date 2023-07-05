#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <system/system-neoled.hpp>
#include <cstdlib>
#include <gscope/gscope.hpp>
#include <bitset>
#include <cstring>

// IN-PI554FCH - https://www.inolux-corp.com/datasheet/SMDLED/Addressable%20LED/IN-PI554FCH.pdf

static constexpr int TOTAL_BITS = 8 * 3;
static uint8_t s_rgb_timer_on_off_periods[2];
static uint8_t s_rgb_timer_data[TOTAL_BITS + 1] = {0};  // add an extra one for the zero at the end

static bool s_sending_data = false;
static bool s_rgb_on = true;

static Neoled_Colour s_colour;
static Neoled_Brightness s_brightness;

static void s_timer_transfer_finish_callback(void*)
{
    // after this time, timer is ready to send a new color
    s_sending_data = false;
}

/**
 * Load float values of rgb into DMA.
 * 
 * @param r Red (0 - 255)
 * @param g Green (0 - 255)
 * @param b Blue (0 - 255)
*/
static void system_neoled_load_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    if (s_sending_data)
    {
        return; // timer is busy sending a new color
    }
    
    s_sending_data = true;

    std::bitset green = std::bitset<8>(g);
    std::bitset red   = std::bitset<8>(r);
    std::bitset blue  = std::bitset<8>(b);

    for (uint8_t i = 0; i < 8; i++)
    {
        s_rgb_timer_data[i] = s_rgb_timer_on_off_periods[green[i]];
        s_rgb_timer_data[i + 8] = s_rgb_timer_on_off_periods[red[i]];
        s_rgb_timer_data[i + 16] = s_rgb_timer_on_off_periods[blue[i]]; 
    }

    hal_timer_neoled_start_dma_transfer(s_rgb_timer_data, TOTAL_BITS + 1);
}
//GScopeCommand("set_led_pwm", system_neoled_load_rgb)

/**
 * Init the RGB LED timings and default Colour & Brightness.
*/
void system_neoled_init()
{
    uint32_t timer_cnt_frequency = hal_tim_neoled_init(s_timer_transfer_finish_callback,  nullptr);
    s_rgb_timer_on_off_periods[0] = timer_cnt_frequency * 0.0000003f;   // 0
    s_rgb_timer_on_off_periods[1] = timer_cnt_frequency * 0.0000006f;   // 1

    s_colour = NEO_WHITE;
    s_brightness = NEO_BRI_10;
}

/**
 * Update the RGB LED, calls the load method based on state whether RGB is on.
*/
void system_neoled_update()
{
    uint32_t now = hal_timer_32_ms();
    static uint32_t s_timestamp;

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;

        if (s_rgb_on)
        {
            system_neoled_load_rgb(
                s_colour.red / s_brightness, 
                s_colour.green / s_brightness,
                s_colour.blue / s_brightness
            );
        }
        else
        {
            system_neoled_load_rgb(0, 0, 0);
        }
    }
}

/**
 * Turn RGB LED on.
*/
void system_neoled_on()
{
    s_rgb_on = true;
}

/**
 * Turn RGB LED off.
*/
void system_neoled_off()
{
    s_rgb_on = false;
}

/**
 * Set the Brightness value of the RGB LED.
 * 
 * @param brightness Neoled_Brightness enum value.
*/
void system_neoled_set_brightness(Neoled_Brightness brightness)
{
    s_brightness = brightness;
}

/**
 * Get the Brightness value of the RGB LED.
 * 
 * @return Neoled Brightness
*/
Neoled_Brightness system_neoled_get_brightness()
{
    return s_brightness;
}

/**
 * Set RGB values of LED
 * 
 * @param r Red
 * @param g Green
 * @param b Blue
*/
void system_neoled_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    s_colour.red = r;
    s_colour.green = g;
    s_colour.blue = b;
}

/**
 * Set the Colour value of the RGB LED.
 * 
 * @param colour Neoled_Colour struct that colour is being changed to.
*/
void system_neoled_set_colour(Neoled_Colour colour)
{
    s_colour = colour;
}

/**
 * Get the current Colour of the RGB LED.
 * 
 * @return Neoled Colour
*/
Neoled_Colour system_neoled_get_colour()
{
    return s_colour;
}
