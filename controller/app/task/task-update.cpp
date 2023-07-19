#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-status.hpp>
#include <gscope/gscope.hpp>
#include <stm-hal/hal-tim.hpp>

static TaskHandle_t s_task_handler;

static void s_update_thread(void*)
{
    while(1)
    {
        system_status_update();

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
}