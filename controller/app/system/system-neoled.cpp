#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
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

static float s_red;
static float s_green;
static float s_blue;

static void s_timer_transfer_finish_callback(void*)
{
    // after this time, timer is ready to send a new color
    s_sending_data = false;
}

/**
 * Load float values of rgb into DMA.
 * 
 * @param r Red
 * @param g Green
 * @param b Blue
*/
static void system_neoled_load_rgb(float r, float g, float b)
{
    if (s_sending_data)
    {
        return; // timer is busy sending a new color
    }
    
    s_sending_data = true;

    uint8_t rgb[3];
    rgb[0] = std::numeric_limits<uint8_t>::max() * g;
    rgb[1] = std::numeric_limits<uint8_t>::max() * r;
    rgb[2] = std::numeric_limits<uint8_t>::max() * b;

    for (int bit_cnt = 0; bit_cnt < TOTAL_BITS; bit_cnt++)
    {
        int offset = bit_cnt / 8;
        int bit = bit_cnt % 8;

        uint8_t enable = (rgb[offset] & bit) != 0 ? 1 : 0;
        s_rgb_timer_data[bit_cnt] = s_rgb_timer_on_off_periods[enable]; // TODO, THIS IS NOTSAFE
    }

    hal_timer_neoled_start_dma_transfer(s_rgb_timer_data, TOTAL_BITS + 1);
}
GScopeCommand("set_led_pwm", system_neoled_load_rgb)

/**
 * Set RGB values of LED
 * 
 * @param r Red
 * @param g Green
 * @param b Blue
*/
void system_neoled_set_rgb(float r, float g, float b)
{
    s_red = r;
    s_green = g;
    s_blue = b;
}

/**
 * Init the RGB LED timings and default colour.
*/
void system_neoled_init()
{
    uint32_t timer_cnt_frequency = hal_tim_neoled_init(s_timer_transfer_finish_callback,  nullptr);
    s_rgb_timer_on_off_periods[0] = timer_cnt_frequency * 0.0000003f;   // 0
    s_rgb_timer_on_off_periods[1] = timer_cnt_frequency * 0.0000006f;   // 1

    s_red = 1.0f;
    s_green = 1.0f;
    s_blue = 1.0f;
}

/**
 * Update the RGB LED, calls the load method based on state of s_rgb_on.
*/
void system_neoled_update()
{
    uint32_t now = hal_timer_32_ms();
    static uint32_t s_timestamp;

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;

        (s_rgb_on)
        ? system_neoled_load_rgb(s_red, s_green, s_blue)
        : system_neoled_load_rgb(0.0f, 0.0f, 0.0f);
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
