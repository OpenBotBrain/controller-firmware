#include <assert.h>
#include <config/appconfig.h>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-exti.hpp>

struct EXTIData
{
    EXTI_HandleTypeDef handler;
    FinishEXTICb cb;
    void* param;
};

static constexpr uint8_t EXTI_TOTAL = 16;
static EXTIData s_exti_data[EXTI_TOTAL];

GPIO_TypeDef* hal_gpio_get_port(uint16_t io);

static constexpr uint32_t s_exti_line[] = {
    EXTI_LINE_0,  EXTI_LINE_1,  EXTI_LINE_2,  EXTI_LINE_3,
    EXTI_LINE_4,  EXTI_LINE_5,  EXTI_LINE_6,  EXTI_LINE_7,
    EXTI_LINE_8,  EXTI_LINE_9,  EXTI_LINE_10,  EXTI_LINE_11,
    EXTI_LINE_12,  EXTI_LINE_13,  EXTI_LINE_14,  EXTI_LINE_15
};

static void exti_handle_interrupt(uint8_t line_start, uint8_t line_end)
{
    for (uint8_t line = line_start; line <= line_end; line++)
    {
        uint32_t flag = s_exti_line[line];
        if (__HAL_GPIO_EXTI_GET_FLAG(flag) && __HAL_GPIO_EXTI_GET_IT(flag))
        {
            __HAL_GPIO_EXTI_CLEAR_FLAG(flag);
            __HAL_GPIO_EXTI_CLEAR_IT(flag);

            EXTIData* data = &s_exti_data[line];
            if (data->cb != nullptr)
            {
                data->cb(data->param);
            }
        }
    }
}

static uint8_t get_offset_from_pin(uint16_t pin)
{
    for (uint8_t i = 0; i < EXTI_TOTAL; i++)
    {
        pin = pin >> 1;
        if (pin == 0)
        {
            return i;
        }
    }
    assert(0);
    return 0;
}

static IRQn_Type get_irq_type(uint16_t pin)
{
    if (pin >= GPIO_PIN_10)
    {
        return EXTI15_10_IRQn;
    }
    else if (pin >= GPIO_PIN_5)
    {
        return EXTI9_5_IRQn;
    }
    else if(pin == GPIO_PIN_4)
    {
        return EXTI4_IRQn;
    }
    else if(pin == GPIO_PIN_3)
    {
        return EXTI3_IRQn;
    }
    else if(pin == GPIO_PIN_2)
    {
        return EXTI2_IRQn;
    }
    else if(pin == GPIO_PIN_1)
    {
        return EXTI1_IRQn;
    }
    else
    {
        return EXTI0_IRQn;
    }
}

uint8_t get_exti_port(GPIO_TypeDef* port)
{
    if (port == GPIOA)
    {
        return EXTI_GPIOA;
    }
    else if (port == GPIOB)
    {
        return EXTI_GPIOB;
    }
    else if (port == GPIOC)
    {
        return EXTI_GPIOC;
    }
    else if (port == GPIOD)
    {
        return EXTI_GPIOD;
    }
    else if (port == GPIOE)
    {
        return EXTI_GPIOE;
    }
    else
    {
        return EXTI_GPIOF;
    }
}


// ------------------------------------------------------------------------
//                              PUBLIC API
// ------------------------------------------------------------------------
void hal_exti_init_default(const BoardSpecificConfig* board_config)
{
    (void) board_config;
}

void hal_exti_init(uint16_t gpio_id, TriggerType trigger, FinishEXTICb cb, void* param)
{
    assert(gpio_id < TOTAL_GPIO);

    uint8_t offset = get_offset_from_pin(hal_gpio_get_pin(gpio_id));

    EXTIData* data = &s_exti_data[offset];
    EXTI_HandleTypeDef* handler = &data->handler;

    data->cb = cb;
    data->param = param;
    handler->Line = s_exti_line[offset];
    handler->PendingCallback = nullptr;

    EXTI_ConfigTypeDef cfg;
    cfg.Line = handler->Line;
    cfg.Mode = EXTI_MODE_INTERRUPT;
    cfg.GPIOSel = get_exti_port(hal_gpio_get_port(gpio_id));

    switch(trigger)
    {
        case TriggerType::RISSING:
            cfg.Trigger = EXTI_TRIGGER_RISING;
            break;
        case TriggerType::FALLING:
            cfg.Trigger = EXTI_TRIGGER_FALLING;
            break;
        case TriggerType::RISSING_FALLING:
            cfg.Trigger = EXTI_TRIGGER_RISING_FALLING;
            break;
    }

    HAL_EXTI_SetConfigLine(handler, &cfg);

    IRQn_Type irq_type = get_irq_type(gpio_id);
    HAL_NVIC_SetPriority(irq_type, PRI_HARD_EXTI, 0);
    HAL_NVIC_EnableIRQ(irq_type);
}

// ------------------------------------------------------------------------
//                             EXTERNAL INTERRUPTIONS
// ------------------------------------------------------------------------
extern "C"
{
    void EXTI0_IRQHandler(void)
    {
        exti_handle_interrupt(0, 0);
    }

    void EXTI1_IRQHandler(void)
    {
        exti_handle_interrupt(1, 1);
    }

    void EXTI2_IRQHandler(void)
    {
        exti_handle_interrupt(2, 2);
    }

    void EXTI3_IRQHandler(void)
    {
        exti_handle_interrupt(3, 3);
    }

    void EXTI4_IRQHandler(void)
    {
        exti_handle_interrupt(4, 4);
    }

    void EXTI9_5_IRQHandler(void)
    {
        exti_handle_interrupt(5, 9);
    }

    void EXTI15_10_IRQHandler(void)
    {
        exti_handle_interrupt(10, 15);
    }
}