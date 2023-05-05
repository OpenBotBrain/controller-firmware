#pragma once

#include <stm32l4xx_hal.h>

enum class AdcType : uint8_t
{
    TypeADC1 = 0,
    TypeADC3
};

struct ADCChannelConfig
{
    uint8_t id;
    AdcType adc;
    uint32_t adc_channel;
};

typedef enum
{
    SET_LOW = 0,
    SET_HIGH,
    DONT_CARE,
} port_state_t;

struct GPIOInitConfig
{
    uint16_t id;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t mode;
    uint8_t pull;
    uint16_t alternate;
    uint16_t speed;
    port_state_t state;
};

struct I2CChannelConfig
{
    uint8_t id;
    I2C_TypeDef* i2c_instance;
    uint32_t timing;
    IRQn_Type irq_type;
    uint8_t irq_priority;
};

struct TimerChannelConfig
{
    uint8_t id;
    TIM_TypeDef* tim;
    uint8_t channel_pri;
    uint8_t channel_sec;
    uint32_t frequency;
};

struct UartChannelConfig
{
    uint8_t id;
    USART_TypeDef* uart;
    uint32_t speed;
    DMA_Channel_TypeDef* tx_channel;
    uint8_t tx_request;
    IRQn_Type tx_irq_type;
    DMA_Channel_TypeDef* rx_channel;
    uint8_t rx_request;
    IRQn_Type rx_irq_type;
    uint8_t irq_priority;
};

struct SPIChannelConfig
{
    uint8_t id;
    SPI_TypeDef* spi;
    IRQn_Type irq_type;
    uint8_t irq_priority;
};

static constexpr uint8_t get_total_channels(const ADCChannelConfig* config, AdcType type, int total)
{
    uint8_t cnt = 0;
    for (int i = 0; i < total; i++)
    {
        if (config[i].adc == type)
        {
            cnt++;
        }
    }
    return cnt;
}

struct BoardSpecificConfig
{
    const char* board_name;
    const uint8_t board_rev;
    const GPIOInitConfig* gpio_config;
    const ADCChannelConfig* adc_config;
    const UartChannelConfig* uart_config;
    const TimerChannelConfig* timer_config;
    const I2CChannelConfig* i2c_config;
    const SPIChannelConfig* spi_config;
    uint8_t total_adc1;
    uint8_t total_adc3;
};

void hal_adc_init_default(const BoardSpecificConfig* board_config);
void hal_exti_init_default(const BoardSpecificConfig* board_config);
void hal_gpio_init_default(const BoardSpecificConfig* board_config);
void hal_i2c_init_default(const BoardSpecificConfig* board_config);
void hal_spi_init_default(const BoardSpecificConfig* board_config);
void hal_tim_init_default(const BoardSpecificConfig* board_config);
void hal_uart_init_default(const BoardSpecificConfig* board_config);