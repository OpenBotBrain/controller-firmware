#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <module/module-ports.hpp>

static TaskHandle_t s_task_handler;

static void s_ports_thread(void*)
{
    while(1)
    {
        module_port_update();

        vTaskDelay(5);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_ports_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_PORTS];

    s_task_handler = xTaskCreateStatic(s_ports_thread, "Ports", SIZE_PORTS,
        0, PRI_PORTS, s_stack, &s_task_buffer);

    module_port_init();
}
