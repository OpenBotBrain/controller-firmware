#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-status.hpp>
#include <system/system-neoled.hpp>
#include <gscope/gscope.hpp>
#include <stm-hal/hal-tim.hpp>

static TaskHandle_t s_task_handler;

static void s_quick_test_update()
{
    static uint32_t s_timestamp;

    uint32_t now = hal_timer_32_ms();

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;
        GSDebug2("Test print %d", now);
    }

}

static void s_update_thread(void*)
{
    while(1)
    {
        system_status_update();

        //system_neoled_update();

        s_quick_test_update();

        vTaskDelay(20);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_update_init()
{
    // Create task for Update App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_UPDATE];

    s_task_handler = xTaskCreateStatic(s_update_thread, "Update", SIZE_UPDATE,
        0, PRI_UPDATE, s_stack, &s_task_buffer);

    system_status_init();
    //system_neoled_init();
}