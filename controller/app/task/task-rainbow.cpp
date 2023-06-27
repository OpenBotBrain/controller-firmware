#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <system/system-led-rgb.hpp>
#include <system/system-neoled.hpp>
#include <cstdint>
#include <stdbool.h>

static TaskHandle_t s_task_handler;
static bool s_led_on = true;

static void s_rainbow_thread(void*)
{
    for( ;; )
    {
        // uint8_t cycles = 0;
        
        // if (cycles == 3)
        // {
        //     system_neoled_reset();
        //     cycles = 0;
        // }

        if (s_led_on)
        {   
            system_neoled_update();
        }
        else
        {
            
        }

        //cycles++;
        vTaskDelay(20);
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
