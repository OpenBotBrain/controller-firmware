#include <app/assert.h>
#include <config/board-rev.hpp>
#include <system/system-freertos.hpp>
#include <task/task-blinky.hpp>
#include <task/task-gscope.hpp>

int main(void)
{
    board_rev_init();               // Init board revision, IOs and clock configuration

    task_blinky_init();             // Init main blinky task.

    task_gscope_init();             // Init gscope debug app

    vTaskStartScheduler();          // Start RTOS

    assert(0);                      // We should never reach this state

    return 0;
}
