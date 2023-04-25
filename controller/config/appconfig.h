#pragma once

#include <stdint.h>

// ------------------------------- FreeRTOS TASK  --------------------------

/***************************************************************************
 * 0: Lowest Priority        ------------------        15: Highes Priority *
 * The priority must be in the range of 0 ----- (configMAX_PRIORITIES - 1) *
 ***************************************************************************/
#define PRI_BLINKY                          1
#define SIZE_BLINKY                         128

#define PRI_GSCOPE                          2
#define SIZE_GSCOPE                         256

// -------------------------- Hardware Interruptions -----------------------
#define PRI_HARD_LPUART1                    8  // lpuart 1 for debug
