#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <hardware/hardware-manager.hpp>
#include <cstdint>
#include <stdbool.h>

static const TickType_t c_tick_delay = 33;

static TaskHandle_t s_task_handler;
static bool s_led_on = false;
static bool s_rgb_on = true;

static HardwareManager s_manager;
static Neoled *s_neoled;
static Led *s_led;

static Neoled_Colour colours[6] =
{
    NEO_RED, NEO_GREEN, NEO_BLUE,
    NEO_YELLOW, NEO_PURPLE, NEO_TEAL,
};

/**
 * Hardware manager task.
 *
 * Flashes the regular LED and cycles the RGB LED.
*/
static void s_hardware_manager_thread(void*)
{
    s_neoled = s_manager.get_neoled();
    s_led = s_manager.get_led();

    s_manager.init();

    for ( ;; )
    {
        for (int i = 0; i < 6; i++)
        {
            s_neoled->set_brightness(NEO_BRI_1);
            s_neoled->set_colour(colours[i]);

            (s_rgb_on) ? s_neoled->on() : s_neoled->off();

            s_neoled->update();

            vTaskDelay(c_tick_delay);
        }

        s_led->set_led_1(!s_led_on);
        s_led->set_led_2(s_led_on);
        s_led->set_led_3(s_led_on);

        s_led_on = !s_led_on;

        s_led->update();
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

void task_hardware_manager_init()
{
    // Create rainbow task
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_HARDWARE_MAN];

    s_task_handler = xTaskCreateStatic(
        s_hardware_manager_thread,
        "Hardware",
        SIZE_HARDWARE_MAN,
        0,
        PRI_HARDWARE_MAN,
        s_stack,
        &s_task_buffer
    );
}
