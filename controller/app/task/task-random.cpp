#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>

static TaskHandle_t s_task_handler;

static void s_random_thread(void*)
{
    for ( ;; )
    {

    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

void task_random_init()
{
    // Create task for Random task
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_RANDOM];

    s_task_handler = xTaskCreateStatic(
        s_random_thread,
        "Random",
        SIZE_RANDOM,
        0,
        PRI_RANDOM,
        s_stack,
        &s_task_buffer
    );
}
