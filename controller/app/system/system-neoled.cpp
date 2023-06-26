#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <cstdlib>

// IN-PI554FCH - https://www.inolux-corp.com/datasheet/SMDLED/Addressable%20LED/IN-PI554FCH.pdf
static uint32_t s_rgb[] = {5000, 8000, 9000};
static int s_bit_cnt = 0;

static void s_timer_update_callback(void*)
{
    int offset = s_bit_cnt / 24;
    int bit = s_bit_cnt % 24;
    if (offset >= 2)
    {
        if (bit >= 23)
        {
            s_bit_cnt = 0;
        }
        else
        {
            s_bit_cnt++;
            hal_tim_neoled_set_reset();
        }
    }
    else
    {
        bool enable = s_rgb[offset] & bit;
        hal_tim_neoled_set_on(enable);
        s_bit_cnt++;
    }
}

void system_neoled_init()
{
    hal_tim_neoled_init(10000, s_timer_update_callback,  nullptr);
}

void system_neoled_update()
{
    uint32_t now = hal_timer_32_ms();
    static uint32_t s_timestamp;

    if ((now - s_timestamp) >= 1000)
    {
        s_rgb[0] = std::rand();
        s_rgb[1] = std::rand();
        s_rgb[2] = std::rand();
    }
}