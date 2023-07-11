#include <bitset>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <system/system-neoled.hpp>
#include <gscope/gscope.hpp>
#include <gscope/gscope.hpp>

// IN-PI554FCH - https://www.inolux-corp.com/datasheet/SMDLED/Addressable%20LED/IN-PI554FCH.pdf

static constexpr int TOTAL_BITS = 8 * 3;
static uint8_t s_rgb_timer_on_off_periods[2];
static uint8_t s_rgb_timer_data[TOTAL_BITS + 1] = {0};  // add an extra one for the zero at the end

static bool s_sending_data = false;

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
        s_rgb_timer_data[i] = s_rgb_timer_on_off_periods[green[7 - i]];
        s_rgb_timer_data[i + 8] = s_rgb_timer_on_off_periods[red[7 - i]];
        s_rgb_timer_data[i + 16] = s_rgb_timer_on_off_periods[blue[7 - i]];
    }

    hal_timer_neoled_start_dma_transfer(s_rgb_timer_data, TOTAL_BITS + 1);
}

/**
 * Init the RGB LED timings and default Colour & Brightness.
*/
void system_neoled_init()
{
    uint32_t timer_cnt_frequency = hal_tim_neoled_init(s_timer_transfer_finish_callback,  nullptr);
    s_rgb_timer_on_off_periods[0] = timer_cnt_frequency * 0.0000003f;   // 0
    s_rgb_timer_on_off_periods[1] = timer_cnt_frequency * 0.0000006f;   // 1
}

/**
 * Update the RGB LED, calls the load method based on state whether RGB is on.
*/
void system_neoled_update(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t now = hal_timer_32_ms();
    static uint32_t s_timestamp;

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;
        system_neoled_load_rgb(r, g, b);
    }
}

static void set_led_pwm_callback(int r, int g, int b)
{
    system_neoled_load_rgb(r, g, b);
}
GScopeCommand("set_led_pwm", set_led_pwm_callback)
