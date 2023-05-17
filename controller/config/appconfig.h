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

#define PRI_POWER_SUPPLY                    3
#define SIZE_POWER_sUPPLY                   256

// -------------------------- Hardware Interruptions -----------------------
#define PRI_HARD_LPUART1                    8   // lpuart 1 for debug
#define PRI_HARD_PORT_UART                  9   // PORT 1 TO 4 uart interruption level
#define PRI_HARD_RPI_UART                   7   // RPI uart com
#define PRI_HARD_ADC                        10  // ADC Dma Priority
#define PRI_HARD_PORT_I2C                   9   // I2C PORT 1 TO 4 interruption level
#define PRI_HARD_SYS_I2C                    11  // System I2C IRQ (LCD, POWER DRIVER)
#define PRI_HARD_EXTI                       12  // External interruptions IO
#define PRI_HARD_SPI                        6   // SPI imu and flash