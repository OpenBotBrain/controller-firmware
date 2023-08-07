#include <app/assert.h>
#include <config/board-rev.hpp>
#include <system/system-freertos.hpp>
#include <task/task-blinky.hpp>
#include <task/task-display.hpp>
#include <task/task-gscope.hpp>
#include <task/task-imu.hpp>
#include <task/task-ports.hpp>
#include <task/task-power-supply.hpp>
#include <task/task-rainbow.hpp>
#include <task/task-hardware-manager.hpp>
#include <task/task-update.hpp>
#include <task/task-memory-game.hpp>
#include <task/task-random.hpp>

int main(void)
{
    board_rev_init();               // Init board revision, IOs and clock configuration

    task_memory_game_init();        // Init memory game.

    // task_random_init();             // Init random task.

    // task_hardware_manager_init();   // Init hardware manager task.

    // task_rainbow_init();            // Init rainbow task.

    // task_blinky_init();             // Init main blinky task.

    task_gscope_init();             // Init gscope debug app

    task_power_supply_init();       // Init power supply system task

    task_ports_init();              // Init main task ports (motor pids, and regular work)

    task_display_init();            // Init main display task

    task_imu_init();                // Init Inertial measurement unit task

    task_update_init();             // Init general system update

    vTaskStartScheduler();          // Start RTOS

    assert(0);                      // We should never reach this state

    return 0;
}
