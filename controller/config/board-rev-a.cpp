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

static constexpr GPIOInitConfig s_gpio_init_rev_a[TOTAL_GPIO] =
{
    //       Name              Port       Pin            Mode             PuPd            AFSel                Speed         initialValue
    { USER_LED1_IO,           GPIOD, GPIO_PIN_7,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0,                   GPIO_SPEED_FREQ_LOW, SET_LOW },
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