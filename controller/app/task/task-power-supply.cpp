#include <config/appconfig.h>
#include <system/system-freertos.hpp>

static TaskHandle_t s_task_handler;

static void s_blinky_thread(void*)
{
    while(1)
    {

        vTaskDelay(500);
    }
}


void task_power_supply_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_POWER_sUPPLY];

    s_task_handler = xTaskCreateStatic(s_blinky_thread, "Power Supply", SIZE_POWER_sUPPLY,
        0, PRI_POWER_SUPPLY, s_stack, &s_task_buffer);
}