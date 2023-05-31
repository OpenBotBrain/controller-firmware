#include <assert.h>
#include <system/system-freertos.hpp>
#include <stm32l4xx_hal.h>

#define IS_IRQ_MASKED()           (__get_PRIMASK() != 0U)
#define IS_IRQ_MODE()             (__get_IPSR() != 0U)
#define IS_IRQ()                  (IS_IRQ_MODE() || (IS_IRQ_MASKED() ))

extern "C" {

void __cxa_pure_virtual() { while (1); }

bool system_freertos_semaphore_take(SemaphoreHandle_t hSemaphore, uint32_t timeout)
{
    bool ret = false;
    if (!IS_IRQ())
    {
        ret = xSemaphoreTake(hSemaphore, timeout) == pdTRUE;
    }
    return ret;
}

void system_freertos_semaphore_give(SemaphoreHandle_t hSemaphore)
{
    if (IS_IRQ())
    {
        BaseType_t yield = pdFALSE;
        xSemaphoreGiveFromISR (hSemaphore, &yield);
        portYIELD_FROM_ISR (yield);
    }
    else
    {
        xSemaphoreGive(hSemaphore);
    }
}

void vApplicationStackOverflowHook (TaskHandle_t xTask, signed char *pcTaskName)
{
    (void) xTask;
    (void) pcTaskName;
    assert(0);
}

#if configSUPPORT_STATIC_ALLOCATION == 1
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#if configUSE_TIMERS == 1
/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}
#endif // configUSE_TIMERS == 1

#endif // configSUPPORT_STATIC_ALLOCATION == 1

// void __cxa_pure_virtual()
// {
//     while (1);
// }

}