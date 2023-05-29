#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>

static TaskHandle_t s_task_handler;
static bool s_enabled = false;

static void s_blinky_thread(void*)
{
    while(1)
    {
        s_enabled = !s_enabled;

        if (s_enabled)
        {
            hal_gpio_set_pin(LED1_IO);
        }
        else
        {
            hal_gpio_reset_pin(LED1_IO);
        }

        vTaskDelay(250);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_blinky_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_BLINKY];

    s_task_handler = xTaskCreateStatic(s_blinky_thread, "Blinky", SIZE_BLINKY,
        0, PRI_BLINKY, s_stack, &s_task_buffer);
}