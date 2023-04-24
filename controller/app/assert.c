#include <app/assert.h>
#include <stm32l4xx_hal.h>

void assert(bool val)
{
    if (!val)
    {
        __disable_irq();
        __asm__("BKPT");
        while(1);
    }
}
