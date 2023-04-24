#pragma once

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>

bool system_freertos_semaphore_take(SemaphoreHandle_t hSemaphore, uint32_t timeout);
void system_freertos_semaphore_give(SemaphoreHandle_t hSemaphore);

#ifdef __cplusplus
}
#endif
