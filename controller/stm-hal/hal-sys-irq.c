// #include "FreeRTOS.h"
// #include "task.h"
#include <assert.h>
#include <stm32l4xx_hal.h>

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
// This function handles Non maskable interrupt.
void NMI_Handler(void)
{

}

// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
    while (1)
    {
        assert(0);
    }
}

// This function handles Memory management fault.
void MemManage_Handler(void)
{
    while (1)
    {
        assert(0);
    }
}

// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
    while (1)
    {
        assert(0);
    }
}


// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
    while (1)
    {
        assert(0);
    }
}

// This function handles Debug monitor.
void DebugMon_Handler(void)
{

}

// This function handles System tick timer.
void SysTick_Handler(void)
{
    HAL_IncTick();
    // if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    // {
    //     xPortSysTickHandler();
    // }
}