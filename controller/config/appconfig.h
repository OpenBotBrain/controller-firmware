#pragma once

#include <stdint.h>

// ------------------------------- FreeRTOS TASK  --------------------------

/***************************************************************************
 * 0: Lowest Priority        ------------------        15: Highes Priority *
 * The priority must be in the range of 0 ----- (configMAX_PRIORITIES - 1) *
 ***************************************************************************/
#define PRI_BLINKY                          1
#define SIZE_BLINKY                         128

#define PRI_RANDOM                          1
#define SIZE_RANDOM                         128

#define PRI_RAINBOW                         1
#define SIZE_RAINBOW                        128

#define PRI_HARDWARE_MAN                    1
#define SIZE_HARDWARE_MAN                   256

#define PRI_GSCOPE                          2
#define SIZE_GSCOPE                         256

#define PRI_POWER_SUPPLY                    3
#define SIZE_POWER_SUPPLY                   256

#define PRI_DISPLAY                         4
#define SIZE_DISPLAY                        256

#define PRI_PORTS                           5
#define SIZE_PORTS                          256

#define PRI_UPDATE                          6
#define SIZE_UPDATE                         256

#define PRI_IMU                             7
#define SIZE_IMU                            256

// -------------------------- Hardware Interruptions -----------------------
#define PRI_HARD_EXTI                       7   // External interruptions IO
#define PRI_HARD_LPUART1                    8   // lpuart 1 for debug
#define PRI_HARD_PORT_UART                  9   // PORT 1 TO 4 uart interruption level
#define PRI_HARD_RPI_UART                   7   // RPI uart com
#define PRI_HARD_ADC                        10  // ADC Dma Priority
#define PRI_HARD_PORT_I2C                   9   // I2C PORT 1 TO 4 interruption level
#define PRI_HARD_SYS_I2C                    11  // System I2C IRQ (LCD, POWER DRIVER)
#define PRI_HARD_SPI                        6   // SPI imu and flash
#define PRI_HARD_TIM5                       4   // NEOPIXEL TIMER