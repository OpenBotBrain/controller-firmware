#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <hardware/hardware-manager.hpp>
#include <cstdint>
#include <stdbool.h>
#include <gscope/gscope-debug.hpp>

static const TickType_t c_tick_delay = 5;

static TaskHandle_t s_task_handler;
static bool s_led_on = false;
static bool s_rgb_on = true;

static HardwareManager s_manager;
static Neoled *s_neoled;
static Led *s_led;
static IMU *s_imu;

static Neoled_Colour s_colour = NEO_WHITE;
static float* s_imu_out;

static float s_roll_threshold = 25.0f;
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

    for ( int i = 0 ;; i++ )
    {
        s_imu->update();
        s_imu_out = s_imu->fetch_roll_pitch();

        if (i % 7 == 0)
        {
            s_neoled->set_brightness(NEO_BRI_1);
            (s_rgb_on) ? s_neoled->on() : s_neoled->off();
            s_colour = (s_imu_out[0] > s_roll_threshold || s_imu_out[0] < -s_roll_threshold) ? NEO_GREEN : NEO_RED;
            s_neoled->set_colour(s_colour);
            s_neoled->update();
        }

        if (i % 50 == 0)
        {
            s_led->set_led_2(!s_led_on);                // top led
            s_led->set_led_1((s_imu_out[0] > -s_roll_threshold));   // middle led
            s_led->set_led_3((s_imu_out[0] < s_roll_threshold));    // bottom led

            s_led_on = !s_led_on;
            s_led->update();
            i = 0;
        }

        vTaskDelay(c_tick_delay);
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
