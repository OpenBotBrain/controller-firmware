#include <app/assert.h>
#include <config/board-rev.h>
#include <stm-hal/hal-board-init.h>

int main(void)
{
    hal_board_init();               // Init board revision, IOs and clock configuration

    assert(0);                      // We should never reach this state

    return 0;
}
