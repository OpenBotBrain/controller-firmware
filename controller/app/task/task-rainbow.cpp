#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <actuator/board-neoled.hpp>
#include <cstdint>
#include <stdbool.h>

static TaskHandle_t s_task_handler;
static bool s_led_on = true;

static void s_rainbow_thread(void*)
{
    Neoled rgb_led;
    rgb_led.init();

    Neoled_Colour colours[6] =
    {
        NEO_RED, NEO_GREEN, NEO_BLUE,
        NEO_YELLOW, NEO_PURPLE, NEO_TEAL,
    };

    for ( ;; )
    {
        // Set lowest Brightness - still bright.
        rgb_led.set_brightness(NEO_BRI_1);

        for(int i = 0 ; i < 6; i++)
        {
            // Set Colour of RGB LED.
            rgb_led.set_colour(colours[i]);

            // Set on state of RGB LED.
            (s_led_on) ? rgb_led.on() : rgb_led.off();

            // Update RGB LED.
            rgb_led.update();

            // Wait for each light interval.
            vTaskDelay(33);
        }
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
