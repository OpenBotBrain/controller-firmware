#include <app/assert.h>

void assert(bool val)
{
    if (!val)
    {
        __disable_irq();
        __asm__("BKPT");
        while(1);
    }
}
