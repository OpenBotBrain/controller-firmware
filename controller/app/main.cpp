#include <app/assert.h>
#include <config/board-rev.hpp>
#include <stm-hal/hal-board-init.h>

int main(void)
{
    board_rev_init();               // Init board revision, IOs and clock configuration

    assert(0);                      // We should never reach this state

    return 0;
}
