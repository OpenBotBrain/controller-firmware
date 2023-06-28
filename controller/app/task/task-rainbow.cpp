#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-neoled.hpp>
#include <cstdint>
#include <stdbool.h>

static TaskHandle_t s_task_handler;
static bool s_led_on = true;

static void s_rainbow_thread(void*)
{
    float colours[3][3] = { 
        { 1.0f, 0.0f, 0.0f }, 
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
    }; 

    for ( ;; )
    {
        for( int i = 0 ; i < 3; i++)
        {   
            // Set Colour of RGB LED.
            system_neoled_set_rgb(colours[i][0], colours[i][1], colours[i][2]);

            // Set on state of RGB LED.
            (s_led_on) ? system_neoled_on() : system_neoled_off();

            // Update RGB LED.
            system_neoled_update();

            vTaskDelay(100);
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

    system_neoled_init();
}
