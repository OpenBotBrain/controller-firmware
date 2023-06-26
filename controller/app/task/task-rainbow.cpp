#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <system/system-led-rgb.hpp>
#include <cstdint>
#include <stdbool.h>

static TaskHandle_t s_task_handler;
static bool led_on = true;

static void s_rainbow_thread(void*)
{
    LEDRGB led_rgb(ADDRESS_LED_RGB_IO);

    for( ;; )
    {
        if (led_on)
        {   
            led_rgb.set_on();
        }
        else
        {
            led_rgb.set_off();
        }

        led_rgb.show();

        // Try to reduce brightness by delaying this bit.
        vTaskDelay(50); 
        led_rgb.write_io(1);

        vTaskDelay(250);

        // Sometimes only turns on when given interrupt, 
        // like from st-info --probe.
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_rainbow_init()
{
    // Create rainbow task
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_RAINBOW];

    s_task_handler = xTaskCreateStatic(
        s_rainbow_thread, 
        "Rainbow", 
        SIZE_RAINBOW,
        0, 
        PRI_RAINBOW, 
        s_stack, 
        &s_task_buffer
    );
}
