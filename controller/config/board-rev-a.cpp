#include <config/appconfig.h>
#include <config/board-rev.hpp>

#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-board-init.hpp>
#include <stm-hal/hal-exti.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-i2c.hpp>
#include <stm-hal/hal-spi.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-uart.hpp>

static constexpr GPIOInitConfig s_gpio_init_rev_a[] =
{
    //           Name                      Config Type    Port       Pin            Mode              PuPd            AFSel                Speed         initialValue
    { INPUT2_PIN6_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_0,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT2_PIN1_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_1,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT3_PIN1_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_2,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN1_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_3,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT3_PIN6_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_4,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN6_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_5,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PIN5_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_6,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORC_PIN5_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_7,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PWM1_IO,                CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_8,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM1,    GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PWM2_IO,                CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_9,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM1,    GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { BUTTON_2_IO,                   CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { USB_DN_IO,                     CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_11, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF10_OTG_FS, GPIO_SPEED_FREQ_VERY_HIGH, DONT_CARE },
    { USB_DP_IO,                     CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_12, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF10_OTG_FS, GPIO_SPEED_FREQ_VERY_HIGH, DONT_CARE },
    { MOTORC_PIN6_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOA, GPIO_PIN_15, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM2,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORA_PIN5_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_0,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORB_PIN5_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_1,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORC_PIN6_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_2,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORC_PIN5_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_3,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM2,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT3_PIN2_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_4,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT3_PIN6_ENABLE_N_IO,       CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_5,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { BUTTON_1_IO,                   CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_6,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { BUTTON_3_IO,                   CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_7,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN6_ENABLE_N_IO,       CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_8,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { INPUT4_PIN2_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_9,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT1_PIN5_IO,                CONFIG_TYPE_I1_INPUT,  GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT1_PIN5_IO,                CONFIG_TYPE_I1_OUTPUT, GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_HIGH,      SET_LOW   },
    { INPUT1_PIN5_IO,                CONGIG_TYPE_I1_SERIAL, GPIOB, GPIO_PIN_10, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART3,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT1_PIN5_IO,                CONFIG_TYPE_I1_I2C,    GPIOB, GPIO_PIN_10, GPIO_MODE_AF_PP,     GPIO_PULLUP,  GPIO_AF3_I2C4,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT1_PIN6_IO,                CONFIG_TYPE_I1_INPUT,  GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT1_PIN6_IO,                CONFIG_TYPE_I1_OUTPUT, GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT1_PIN6_IO,                CONGIG_TYPE_I1_SERIAL, GPIOB, GPIO_PIN_11, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART3,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT1_PIN6_IO,                CONFIG_TYPE_I1_I2C,    GPIOB, GPIO_PIN_11, GPIO_MODE_AF_PP,     GPIO_PULLUP,  GPIO_AF3_I2C4,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { SYS_IMU_IRQ2_IO,               CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_12, GPIO_MODE_IT_FALLING,GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { SYS_IMU_IRQ1_IO,               CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_13, GPIO_MODE_IT_FALLING,GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PIN6_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_14, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF14_TIM15,  GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORD_PIN5_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOB, GPIO_PIN_15, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF14_TIM15,  GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { SYS_I2C_SCL_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_0,  GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C3,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { SYS_I2C_SDA_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_1,  GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C3,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORC_PIN6_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_2,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PIN6_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_3,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORB_PIN6_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_4,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORA_PIN6_SENSE_IO,          CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_5,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORB_PWM1_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_6,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF3_TIM8,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORB_PWM2_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_7,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF3_TIM8,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORA_PWM1_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_8,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF3_TIM8,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORA_PWM2_IO,                CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_9,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF3_TIM8,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT2_PIN5_IO,                CONFIG_TYPE_I2_INPUT,  GPIOC, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN5_SCL_IO
    { INPUT2_PIN5_IO,                CONFIG_TYPE_I2_OUTPUT, GPIOC, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN5_SCL_IO
    { INPUT2_PIN5_IO,                CONGIG_TYPE_I2_SERIAL, GPIOC, GPIO_PIN_10, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_UART4,   GPIO_SPEED_FREQ_HIGH,      DONT_CARE }, // IO is shared with INPUT2_PIN5_SCL_IO
    { INPUT2_PIN5_IO,                CONFIG_TYPE_I2_I2C,    GPIOC, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN5_SCL_IO
    { INPUT2_PIN6_IO,                CONFIG_TYPE_I2_INPUT,  GPIOC, GPIO_PIN_11, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN6_SDA_IO
    { INPUT2_PIN6_IO,                CONFIG_TYPE_I2_OUTPUT, GPIOC, GPIO_PIN_11, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN6_SDA_IO
    { INPUT2_PIN6_IO,                CONGIG_TYPE_I2_SERIAL, GPIOC, GPIO_PIN_11, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_UART4,   GPIO_SPEED_FREQ_HIGH,      DONT_CARE }, // IO is shared with INPUT2_PIN6_SDA_IO
    { INPUT2_PIN6_IO,                CONFIG_TYPE_I2_I2C,    GPIOC, GPIO_PIN_11, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT2_PIN6_SDA_IO
    { PC_UTX_RPIRX_IO,               CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_12, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_UART5,   GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { LED3_IO,                       CONFIG_TYPE_DEFAULT,   GPIOC, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT1_PIN1_CURRENT_ENABLE_IO, CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_0,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT1_PIN6_ENABLE_N_IO,       CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_1,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { PC_URX_RPITX_IO,               CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_2,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_UART5,   GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT1_PIN2_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_3,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT2_PIN2_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_4,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT3_PIN5_IO,                CONFIG_TYPE_I3_INPUT,  GPIOF, GPIO_PIN_5,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN5_SCL_IO
    { INPUT3_PIN5_IO,                CONFIG_TYPE_I3_OUTPUT, GPIOF, GPIO_PIN_5,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   }, // IO is shared with INPUT3_PIN5_SCL_IO
    { INPUT3_PIN5_IO,                CONGIG_TYPE_I3_SERIAL, GPIOD, GPIO_PIN_5,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART2,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE }, // IO is shared with INPUT3_PIN5_SCL_IO
    { INPUT3_PIN5_IO,                CONFIG_TYPE_I3_I2C,    GPIOF, GPIO_PIN_5,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN5_SCL_IO
    { INPUT3_PIN6_IO,                CONFIG_TYPE_I3_INPUT,  GPIOF, GPIO_PIN_6,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN6_SDA_IO
    { INPUT3_PIN6_IO,                CONFIG_TYPE_I3_OUTPUT, GPIOF, GPIO_PIN_6,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   }, // IO is shared with INPUT3_PIN6_SDA_IO
    { INPUT3_PIN6_IO,                CONGIG_TYPE_I3_SERIAL, GPIOD, GPIO_PIN_6,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART2,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE }, // IO is shared with INPUT3_PIN6_SDA_IO
    { INPUT3_PIN6_IO,                CONFIG_TYPE_I3_I2C,    GPIOF, GPIO_PIN_6,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN6_SDA_IO
    { LED1_IO,                       CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_7,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { MOTORAB_SLEEP_N_IO,            CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_8,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { MOTORAB_FAULT_N_IO,            CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_9,  GPIO_MODE_INPUT,     GPIO_PULLUP,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { SHUTDOWN_IO,                   CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { BM_NIRQ_IO,                    CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_11, GPIO_MODE_IT_FALLING,GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORA_PIN6_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_12, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF2_TIM4,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORA_PIN5_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_13, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF2_TIM4,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { PM_NIRQ_IO,                    CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_14, GPIO_MODE_IT_FALLING,GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { SYS_FLASH_CS_N_IO,             CONFIG_TYPE_DEFAULT,   GPIOD, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { PIEZO_PWM_IO,                  CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_0,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN1_CURRENT_ENABLE_IO, CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_1,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { LED2_IO,                       CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_2,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { MOTORB_PIN5_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_3,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF2_TIM3,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORB_PIN6_ENCODER_IO,        CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_4,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF2_TIM3,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORCD_FAULT_N_IO,            CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_7,  GPIO_MODE_INPUT,     GPIO_PULLUP,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORB_PIN5_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORB_PIN6_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_11, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORA_PIN5_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_12, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORC_PWM2_IO,                CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_13, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM1,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORC_PWM1_IO,                CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_14, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF1_TIM1,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { MOTORA_PIN6_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOE, GPIO_PIN_15, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT3_PIN6_SDA_IO,            CONFIG_TYPE_I3_I2C,    GPIOF, GPIO_PIN_0,  GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C2,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT3_PIN6_SDA_IO,            CONFIG_TYPE_I3_INPUT,  GPIOF, GPIO_PIN_0,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN6_IO
    { INPUT3_PIN6_SDA_IO,            CONFIG_TYPE_I3_OUTPUT, GPIOF, GPIO_PIN_0,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN6_IO
    { INPUT3_PIN6_SDA_IO,            CONGIG_TYPE_I3_SERIAL, GPIOF, GPIO_PIN_0,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN6_IO
    { INPUT3_PIN5_SCL_IO,            CONFIG_TYPE_I3_I2C,    GPIOF, GPIO_PIN_1,  GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C2,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT3_PIN5_SCL_IO,            CONFIG_TYPE_I3_INPUT,  GPIOF, GPIO_PIN_1,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN5_IO
    { INPUT3_PIN5_SCL_IO,            CONFIG_TYPE_I3_OUTPUT, GPIOF, GPIO_PIN_1,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN5_IO
    { INPUT3_PIN5_SCL_IO,            CONGIG_TYPE_I3_SERIAL, GPIOF, GPIO_PIN_1,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE }, // IO is shared with INPUT3_PIN5_IO
    { ADC_3V3_SENSE_IO,              CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_3,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { ADC_9V_SENSE_IO,               CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_4,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { ADC_5V_SENSE_IO,               CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_5,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { ADC_VBAT_SENSE_IO,             CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_6,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { ADC_VBUS_SENSE_IO,             CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_7,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { ADDRESS_LED_RGB_IO,            CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_8,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT1_PIN1_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_9,  GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT1_PIN6_SENSE_VOLTAGE_IO,  CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_10, GPIO_MODE_ANALOG,    GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PIN6_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_11, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORD_PIN5_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_12, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORC_PIN5_DETECT_IO,         CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { MOTORCD_SLEEP_N_IO,            CONFIG_TYPE_DEFAULT,   GPIOF, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { SYS_SPI_CLK_IO,                CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_2,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF5_SPI1,    GPIO_SPEED_FREQ_VERY_HIGH, DONT_CARE },
    { SYS_SPI_MISO_IO,               CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_3,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF5_SPI1,    GPIO_SPEED_FREQ_VERY_HIGH, DONT_CARE },
    { SYS_SPI_MOSI_IO,               CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_4,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF5_SPI1,    GPIO_SPEED_FREQ_VERY_HIGH, DONT_CARE },
    { SYS_IMU_CS_IO,                 CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_5,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { VBAT_ADC_ON_IO,                CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_6,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { DEBUG_UTX_DRX_IO,              CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_7,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_LPUART1, GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { DEBUG_URX_DTX_IO,              CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_8,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF8_LPUART1, GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT4_PIN5_IO,                CONFIG_TYPE_I4_INPUT,  GPIOG, GPIO_PIN_9,  GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN5_IO,                CONFIG_TYPE_I4_OUTPUT, GPIOG, GPIO_PIN_9,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT4_PIN5_IO,                CONGIG_TYPE_I4_SERIAL, GPIOG, GPIO_PIN_9,  GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART1,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT4_PIN6_IO,                CONFIG_TYPE_I4_INPUT,  GPIOG, GPIO_PIN_10, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       DONT_CARE },
    { INPUT4_PIN6_IO,                CONFIG_TYPE_I4_OUTPUT, GPIOG, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT4_PIN6_IO,                CONGIG_TYPE_I4_SERIAL, GPIOG, GPIO_PIN_10, GPIO_MODE_AF_PP,     GPIO_NOPULL,  GPIO_AF7_USART1,  GPIO_SPEED_FREQ_HIGH,      DONT_CARE },
    { INPUT2_PIN1_CURRENT_ENABLE_IO, CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
    { INPUT2_PIN6_ENABLE_N_IO,       CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_HIGH  },
    { INPUT2_PIN6_SDA_IO,            CONFIG_TYPE_I2_INPUT,  GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN6_IO
    { INPUT2_PIN6_SDA_IO,            CONFIG_TYPE_I2_OUTPUT, GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN6_IO
    { INPUT2_PIN6_SDA_IO,            CONGIG_TYPE_I2_SERIAL, GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN6_IO
    { INPUT2_PIN6_SDA_IO,            CONFIG_TYPE_I2_I2C,    GPIOG, GPIO_PIN_13, GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C1,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT2_PIN5_SCL_IO,            CONFIG_TYPE_I2_INPUT,  GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN5_IO
    { INPUT2_PIN5_SCL_IO,            CONFIG_TYPE_I2_OUTPUT, GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN5_IO
    { INPUT2_PIN5_SCL_IO,            CONGIG_TYPE_I2_SERIAL, GPIOG, GPIO_PIN_13, GPIO_MODE_INPUT,     GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE }, // It is shared with INPUT2_PIN5_IO
    { INPUT2_PIN5_SCL_IO,            CONFIG_TYPE_I2_I2C,    GPIOG, GPIO_PIN_14, GPIO_MODE_AF_OD,     GPIO_NOPULL,  GPIO_AF4_I2C1,    GPIO_SPEED_FREQ_MEDIUM,    DONT_CARE },
    { INPUT3_PIN1_CURRENT_ENABLE_IO, CONFIG_TYPE_DEFAULT,   GPIOG, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,  0,                GPIO_SPEED_FREQ_LOW,       SET_LOW   },
};

static constexpr ADCChannelConfig s_adc_config[ADC_CHANNEL_TOTAL] =
{
    { ADC_CHANNEL_TYPE_PORT_OUTPUT3_PIN6, AdcType::TypeADC1, ADC_CHANNEL_3  },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT4_PIN6, AdcType::TypeADC1, ADC_CHANNEL_4  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT2_PIN6,  AdcType::TypeADC1, ADC_CHANNEL_5  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT2_PIN1,  AdcType::TypeADC1, ADC_CHANNEL_6  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT3_PIN1,  AdcType::TypeADC1, ADC_CHANNEL_7  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT4_PIN1,  AdcType::TypeADC1, ADC_CHANNEL_8  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT3_PIN6,  AdcType::TypeADC1, ADC_CHANNEL_9  },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT4_PIN6,  AdcType::TypeADC1, ADC_CHANNEL_10 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT4_PIN5, AdcType::TypeADC1, ADC_CHANNEL_11 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT3_PIN5, AdcType::TypeADC1, ADC_CHANNEL_12 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT2_PIN6, AdcType::TypeADC1, ADC_CHANNEL_13 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT1_PIN6, AdcType::TypeADC1, ADC_CHANNEL_14 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT1_PIN5, AdcType::TypeADC1, ADC_CHANNEL_15 },  //
    { ADC_CHANNEL_TYPE_PORT_OUTPUT2_PIN5, AdcType::TypeADC1, ADC_CHANNEL_16 },  //
    { ADC_CHANNEL_TYPE_3V3_SENSE,         AdcType::TypeADC3, ADC_CHANNEL_6  },  //
    { ADC_CHANNEL_TYPE_9V_SENSE,          AdcType::TypeADC3, ADC_CHANNEL_7  },  //
    { ADC_CHANNEL_TYPE_5V_SENSE,          AdcType::TypeADC3, ADC_CHANNEL_8  },  //
    { ADC_CHANNEL_TYPE_VBAT_SENSE,        AdcType::TypeADC3, ADC_CHANNEL_9  },  //
    { ADC_CHANNEL_TYPE_VBUS_SENSE,        AdcType::TypeADC3, ADC_CHANNEL_10 },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT1_PIN1,  AdcType::TypeADC3, ADC_CHANNEL_12 },  //
    { ADC_CHANNEL_TYPE_PORT_INPUT1_PIN6,  AdcType::TypeADC3, ADC_CHANNEL_13 },  //
    { ADC_CHANNEL_TYPE_CORE_TEMP,         AdcType::TypeADC3, ADC_CHANNEL_17 },
};

static constexpr UartChannelConfig s_uart_config[UART_TYPE_TOTAL] =
{
    { UART_TYPE_DEBUG_SERIAL, LPUART1, 921600, DMA2_Channel6, DMA_REQUEST_4, DMA2_Channel6_IRQn, DMA2_Channel7, DMA_REQUEST_4, DMA2_Channel7_IRQn, PRI_HARD_LPUART1   },
    { UART_TYPE_PORT_INPUT_1, USART3,  115200, DMA1_Channel2, DMA_REQUEST_2, DMA1_Channel2_IRQn, DMA1_Channel3, DMA_REQUEST_2, DMA1_Channel3_IRQn, PRI_HARD_PORT_UART },
    { UART_TYPE_PORT_INPUT_2, UART4,   115200, DMA2_Channel3, DMA_REQUEST_2, DMA2_Channel3_IRQn, nullptr,       0,             UART4_IRQn,         PRI_HARD_PORT_UART },
    { UART_TYPE_PORT_INPUT_3, USART2,  115200, DMA1_Channel7, DMA_REQUEST_2, DMA1_Channel7_IRQn, DMA1_Channel6, DMA_REQUEST_2, DMA1_Channel6_IRQn, PRI_HARD_PORT_UART },
    { UART_TYPE_PORT_INPUT_4, USART1,  115200, DMA1_Channel4, DMA_REQUEST_2, DMA1_Channel4_IRQn, DMA1_Channel5, DMA_REQUEST_2, DMA1_Channel5_IRQn, PRI_HARD_PORT_UART },
    { UART_TYPE_PORT_RPI,     UART5,   921600, DMA2_Channel1, DMA_REQUEST_2, DMA2_Channel1_IRQn, DMA2_Channel2, DMA_REQUEST_2, DMA2_Channel2_IRQn, PRI_HARD_RPI_UART  },
};

static constexpr TimerChannelConfig s_timer_config[TIMER_TYPE_TOTAL] =
{
    { TIMER_TYPE_MOTOR_A_PWM,     TIM8,  TIM_CHANNEL_3, TIM_CHANNEL_4, 10000 },
    { TIMER_TYPE_MOTOR_B_PWM,     TIM8,  TIM_CHANNEL_1, TIM_CHANNEL_2, 10000 },
    { TIMER_TYPE_MOTOR_C_PWM,     TIM1,  TIM_CHANNEL_3, TIM_CHANNEL_4, 10000 },
    { TIMER_TYPE_MOTOR_D_PWM,     TIM1,  TIM_CHANNEL_1, TIM_CHANNEL_2, 10000 },
    { TIMER_TYPE_MOTOR_A_ENCODER, TIM4,  TIM_CHANNEL_1, TIM_CHANNEL_2, 0     },
    { TIMER_TYPE_MOTOR_B_ENCODER, TIM3,  TIM_CHANNEL_1, TIM_CHANNEL_2, 0     },
    { TIMER_TYPE_MOTOR_C_ENCODER, TIM2,  TIM_CHANNEL_1, TIM_CHANNEL_2, 0     },
    { TIMER_TYPE_MOTOR_D_ENCODER, TIM15, TIM_CHANNEL_1, TIM_CHANNEL_2, 0     },
    { TIMER_TYPE_PIEZO_PWM,       TIM16, TIM_CHANNEL_1, 0            , 10000 },
};

static constexpr I2CChannelConfig s_i2c_config[I2C_TYPE_TOTAL] =
{
    { I2C_TYPE_PORT_INPUT_1, I2C4, 0x10909CEC, I2C1_EV_IRQn, PRI_HARD_PORT_I2C },
    { I2C_TYPE_PORT_INPUT_2, I2C1, 0x10909CEC, I2C2_EV_IRQn, PRI_HARD_PORT_I2C },
    { I2C_TYPE_PORT_INPUT_3, I2C2, 0x10909CEC, I2C3_EV_IRQn, PRI_HARD_PORT_I2C },
    { I2C_TYPE_SYSTEM,       I2C3, 0x10909CEC, I2C4_EV_IRQn, PRI_HARD_SYS_I2C  },
};

static constexpr SPIChannelConfig s_spi_config[SPI_TOTAL] =
{
    { SPI_TYPE_IMU_FLASH, SPI1, SPI1_IRQn, PRI_HARD_SPI },
};

static constexpr BoardSpecificConfig s_rev_a_specific_config =
{
    .board_name = "REV A",
    .board_rev = BOARD_REV_A,
    .gpio_config = s_gpio_init_rev_a,
    .gpio_total = sizeof(s_gpio_init_rev_a) / sizeof(GPIOInitConfig),
    .adc_config = s_adc_config,
    .uart_config = s_uart_config,
    .timer_config = s_timer_config,
    .i2c_config = s_i2c_config,
    .spi_config = s_spi_config,
    .total_adc1 = get_total_channels(s_adc_config, AdcType::TypeADC1, ADC_CHANNEL_TOTAL),
    .total_adc3 = get_total_channels(s_adc_config, AdcType::TypeADC3, ADC_CHANNEL_TOTAL),
};

const BoardSpecificConfig* board_rev_a_get_specific_config()
{
    return &s_rev_a_specific_config;
}