#include <assert.h>
#include <config/appconfig.h>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-tim.hpp>

struct TimerConfig
{
    uint8_t id;
    TIM_TypeDef* tim;
    uint8_t channel_pri;
    uint8_t channel_sec;
    uint32_t frequency;
};

struct TimerData
{
    TIM_HandleTypeDef handler;
    uint16_t period;
};

static constexpr TimerConfig s_timer_config[TIMER_TYPE_TOTAL] =
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

static TimerData s_timer_data[TIMER_TYPE_TOTAL];
static uint32_t s_timer_us_cnt = 0;
static uint32_t s_timer_ms_cnt = 0;
static TIM_HandleTypeDef s_tim6;    // ADC Timer
static TIM_HandleTypeDef s_tim7;    // us timer

static void adc_timer_init()
{
    __HAL_RCC_TIM6_CLK_ENABLE();

    s_tim6.Instance = TIM6;
    s_tim6.Init.Prescaler = 0;
    s_tim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_tim6.Init.Period = HAL_RCC_GetPCLK1Freq() / hal_adc_get_timer_sample_frequency();
    s_tim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    assert (HAL_TIM_Base_Init(&s_tim6) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(&s_tim6, &sMasterConfig) == HAL_OK);

    HAL_TIM_Base_Start(&s_tim6);
}

static void us_timer_init()
{
    __HAL_RCC_TIM7_CLK_ENABLE();

    s_tim7.Instance = TIM7;
    s_tim7.Init.Prescaler = 0;
    s_tim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_tim7.Init.Period = HAL_RCC_GetPCLK1Freq() / 1000000;
    s_tim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    assert (HAL_TIM_Base_Init(&s_tim7) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(&s_tim7, &sMasterConfig) == HAL_OK);

    HAL_TIM_Base_Start(&s_tim7);

    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);    // High priority!
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    __HAL_TIM_ENABLE_IT(&s_tim7, TIM_IT_UPDATE);
}

void hal_tim_init_default(uint8_t board_rev)
{
    (void) board_rev;

    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_TIM7_CLK_ENABLE();
    __HAL_RCC_TIM8_CLK_ENABLE();
    __HAL_RCC_TIM15_CLK_ENABLE();
    __HAL_RCC_TIM16_CLK_ENABLE();

    adc_timer_init();
    us_timer_init();
}

void hal_tim_pwm_init(uint8_t type)
{
    assert(type < TIMER_TYPE_TOTAL);

    const TimerConfig* config = &s_timer_config[type];
    TimerData* data = &s_timer_data[type];
    TIM_HandleTypeDef* handler = &data->handler;

    data->period = (HAL_RCC_GetPCLK1Freq() / config->frequency);

    handler->Instance = config->tim;
    handler->Init.Prescaler = 0;
    handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    handler->Init.Period = data->period;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.RepetitionCounter = 0;
    handler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    assert(HAL_TIM_Base_Init(handler) == HAL_OK);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    assert(HAL_TIM_ConfigClockSource(handler, &sClockSourceConfig) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(handler, &sMasterConfig) == HAL_OK);

    assert(HAL_TIM_OC_Init(handler) == HAL_OK);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    assert(HAL_TIM_OC_ConfigChannel(handler, &sConfigOC, config->channel_pri) == HAL_OK);
    __HAL_TIM_SET_COMPARE(handler, config->channel_pri, 0);
    HAL_TIM_OC_Start(handler, config->channel_pri);

    if (config->channel_sec != 0)
    {
        assert(HAL_TIM_OC_ConfigChannel(handler, &sConfigOC, config->channel_sec) == HAL_OK);
        __HAL_TIM_SET_COMPARE(handler, config->channel_sec, 0);
        HAL_TIM_OC_Start(handler, config->channel_sec);
    }
}

void hal_tim_pwm_set_pwm(uint8_t type, float duty)
{
    assert(type < TIMER_TYPE_TOTAL);

    TimerData* data = &s_timer_data[type];

    if (duty > 1.0f)
    {
        duty = 1.0f;
    }
    else if (duty < -1.0f)
    {
        duty = -1.0f;
    }

    // TODO! Work with negative values!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    assert(duty >= 0.0);

    uint16_t new_period = duty * data->period;

    __HAL_TIM_SET_AUTORELOAD(&data->handler, new_period);
}

void hal_tim_encoder_init(uint8_t type)
{
    assert(type < TIMER_TYPE_TOTAL);

    const TimerConfig* config = &s_timer_config[type];
    TimerData* data = &s_timer_data[type];
    TIM_HandleTypeDef* handler = &data->handler;

    handler->Instance = config->tim;
    handler->Init.Prescaler = 0;
    handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    handler->Init.Period = 65535;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.RepetitionCounter = 0;
    handler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    assert(HAL_TIM_IC_Init(handler) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(handler, &sMasterConfig) == HAL_OK);

    // TODO: THIS CONFIGURATION IT IS NOT CORRECT!!!
    TIM_IC_InitTypeDef sConfigIC = {0};
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    assert(HAL_TIM_IC_ConfigChannel(handler, &sConfigIC, config->channel_pri) == HAL_OK);
    assert(HAL_TIM_IC_ConfigChannel(handler, &sConfigIC, config->channel_sec) == HAL_OK);

    assert(HAL_TIM_IC_Start(handler, config->channel_pri) == HAL_OK);
    assert(HAL_TIM_IC_Start(handler, config->channel_sec) == HAL_OK);
}

uint32_t hal_tim_encoder_get_tick(uint8_t type)
{
    assert(type < TIMER_TYPE_TOTAL);
    TimerData* data = &s_timer_data[type];

    return __HAL_TIM_GET_COUNTER(&data->handler);
}

extern "C" {

void TIM7_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&s_tim7, TIM_FLAG_UPDATE) != RESET &&
        __HAL_TIM_GET_IT_SOURCE(&s_tim7, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&s_tim7, TIM_IT_UPDATE);
        s_timer_us_cnt += 65536;
    }
}

void HAL_IncTick(void)
{
    s_timer_ms_cnt++;
}

uint32_t HAL_GetTick(void)
{
    return s_timer_ms_cnt;
}

uint32_t hal_timer_32_ms(void)
{
    return s_timer_ms_cnt;
}

uint32_t hal_timer_32_us(void)
{
    uint32_t now;

    __disable_irq();
    now = TIM7->CNT;
    now += s_timer_us_cnt;
    __enable_irq();

    return now;
}

}

