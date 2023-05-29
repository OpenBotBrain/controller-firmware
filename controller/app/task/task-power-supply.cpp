#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-status.hpp>
#include <stm-hal/hal-tim.hpp>
#include <gscope/gscope.hpp>

static TaskHandle_t s_task_handler;

static void s_debug_produce_update()
{
    static uint32_t s_timestamp = 0;
    uint32_t now = hal_timer_32_ms();

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;
    }
}

static void s_blinky_thread(void*)
{
    while(1)
    {
        s_debug_produce_update();

        system_status_update();

        vTaskDelay(25);
    }
}


void task_power_supply_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_POWER_sUPPLY];

    s_task_handler = xTaskCreateStatic(s_blinky_thread, "Power Supply", SIZE_POWER_sUPPLY,
        0, PRI_POWER_SUPPLY, s_stack, &s_task_buffer);

    system_status_init();
}