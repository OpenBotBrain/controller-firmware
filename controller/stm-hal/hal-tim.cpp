#include <assert.h>
#include <config/appconfig.h>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-datatype.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-exti.hpp>

struct TimerData
{
    TIM_HandleTypeDef handler;
    uint16_t period;
};

static const TimerChannelConfig* s_timer_config;
static TimerData s_timer_data[TIMER_TYPE_TOTAL];
static uint32_t s_timer_us_cnt = 0;
static uint32_t s_timer_ms_cnt = 0;
static TIM_HandleTypeDef s_tim6;    // ADC Timer
static TIM_HandleTypeDef s_tim7;    // us timer
static uint16_t s_timer15_local_counter = 0;

static TIM_HandleTypeDef s_tim5;
DMA_HandleTypeDef s_hdma_tim5_ch3_up;
static TimerUpdateCb s_timer_neoled_update_cb = nullptr;
static void* s_neoled_param;

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
    s_tim7.Init.Prescaler = HAL_RCC_GetPCLK1Freq() / 1000000;
    s_tim7.Init.Period = 65535;
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

void hal_tim_init_default(const BoardSpecificConfig* board_config)
{
    s_timer_config = board_config->timer_config;
    assert(s_timer_config);

    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_TIM5_CLK_ENABLE();
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

    const TimerChannelConfig* config = &s_timer_config[type];
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
    const TimerChannelConfig* config = &s_timer_config[type];

    if (duty > 1.0f)
    {
        duty = 1.0f;
    }
    else if (duty < -1.0f)
    {
        duty = -1.0f;
    }

    float in1 = 0.0f;
    float in2 = 0.0f;
    if (duty >= 0)
    {
        in1 = duty;
    }
    else
    {
        in2 = -duty;
    }

    __HAL_TIM_SET_COMPARE(&data->handler, config->channel_pri, static_cast<uint16_t>(in1 * data->period));
    __HAL_TIM_SET_COMPARE(&data->handler, config->channel_sec, static_cast<uint16_t>(in2 * data->period));
}

static void s_encoder_tim15_rissing_edge_pin5(void*)
{
    if (hal_gpio_read_pin(MOTORD_PIN6_ENCODER_IO))
    {
        s_timer15_local_counter++;
    }
    else
    {
        s_timer15_local_counter--;
    }
}

static void s_encoder_tim15_rissing_edge_pin6(void*)
{
    if (!hal_gpio_read_pin(MOTORD_PIN5_ENCODER_IO))
    {
        s_timer15_local_counter++;
    }
    else
    {
        s_timer15_local_counter--;
    }
}

void hal_tim_encoder_init(uint8_t type)
{
    assert(type < TIMER_TYPE_TOTAL);

    const TimerChannelConfig* config = &s_timer_config[type];
    TimerData* data = &s_timer_data[type];
    TIM_HandleTypeDef* handler = &data->handler;

    if (config->tim == TIM15)
    {
        // Special case, does not support encoder, do it via software
        hal_exti_init(MOTORD_PIN5_ENCODER_IO, TriggerType::RISSING, s_encoder_tim15_rissing_edge_pin5, nullptr);
        hal_exti_init(MOTORD_PIN6_ENCODER_IO, TriggerType::RISSING, s_encoder_tim15_rissing_edge_pin6, nullptr);
        return;
    }

    handler->Instance = config->tim;
    handler->Init.Prescaler = 0;
    handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    handler->Init.Period = 65535;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.RepetitionCounter = 0;
    handler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    assert(HAL_TIM_IC_Init(handler) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(handler, &sMasterConfig) == HAL_OK);

    TIM_Encoder_InitTypeDef sConfig = {0};
    sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 10;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 10;
    assert(HAL_TIM_Encoder_Init(handler, &sConfig) == HAL_OK);

    assert(HAL_TIM_Encoder_Start(handler, config->channel_pri) == HAL_OK);
    assert(HAL_TIM_Encoder_Start(handler, config->channel_sec) == HAL_OK);
}

uint32_t hal_tim_encoder_get_tick(uint8_t type)
{
    assert(type < TIMER_TYPE_TOTAL);
    TimerData* data = &s_timer_data[type];

    if (s_timer_config[type].tim == TIM15)
    {
        return s_timer15_local_counter;
    }

    return __HAL_TIM_GET_COUNTER(&data->handler);
}

static uint32_t s_timer_cnt_frequency;
static constexpr uint32_t NEOPIXEL_FREQUENCY_HZ = 833000;
uint32_t hal_tim_neoled_init(TimerUpdateCb cb, void* param)
{
    s_timer_neoled_update_cb = cb;
    s_neoled_param = param;

    s_timer_cnt_frequency = HAL_RCC_GetPCLK1Freq();
    s_tim5.Instance = TIM5;
    s_tim5.Init.Prescaler = 0;
    s_tim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_tim5.Init.Period = s_timer_cnt_frequency / NEOPIXEL_FREQUENCY_HZ;
    s_tim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    assert (HAL_TIM_PWM_Init(&s_tim5) == HAL_OK);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    assert(HAL_TIMEx_MasterConfigSynchronization(&s_tim5, &sMasterConfig) == HAL_OK);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    assert (HAL_TIM_PWM_ConfigChannel(&s_tim5, &sConfigOC, TIM_CHANNEL_3) == HAL_OK);

    s_hdma_tim5_ch3_up.Instance = DMA2_Channel2;
    s_hdma_tim5_ch3_up.Init.Request = DMA_REQUEST_5;
    s_hdma_tim5_ch3_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
    s_hdma_tim5_ch3_up.Init.PeriphInc = DMA_PINC_DISABLE;
    s_hdma_tim5_ch3_up.Init.MemInc = DMA_MINC_ENABLE;
    s_hdma_tim5_ch3_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    s_hdma_tim5_ch3_up.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    s_hdma_tim5_ch3_up.Init.Mode = DMA_NORMAL;
    s_hdma_tim5_ch3_up.Init.Priority = DMA_PRIORITY_MEDIUM;
    assert (HAL_DMA_Init(&s_hdma_tim5_ch3_up) == HAL_OK);

    // Several peripheral DMA handle pointers point to the same DMA handle.
    // Be aware that there is only one channel to perform all the requested DMAs.
    __HAL_LINKDMA(&s_tim5, hdma[TIM_DMA_ID_CC3],s_hdma_tim5_ch3_up);
    __HAL_LINKDMA(&s_tim5, hdma[TIM_DMA_ID_UPDATE],s_hdma_tim5_ch3_up);

    HAL_TIM_PWM_Stop_DMA(&s_tim5, TIM_CHANNEL_3);

    HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, PRI_HARD_TIM5, 0);    // High priority!
    HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
    __HAL_DMA_ENABLE_IT(&s_hdma_tim5_ch3_up, DMA_IT_TC);

    HAL_NVIC_SetPriority(TIM5_IRQn, PRI_HARD_TIM5, 0);    // High priority!
    HAL_NVIC_EnableIRQ(TIM5_IRQn);

    return s_timer_cnt_frequency;
}

void hal_timer_neoled_start_dma_transfer(uint8_t* data, uint32_t size)
{
    HAL_TIM_PWM_Stop_DMA(&s_tim5, TIM_CHANNEL_3);
    s_tim5.Init.Period = s_timer_cnt_frequency / NEOPIXEL_FREQUENCY_HZ;
    __HAL_TIM_SET_AUTORELOAD(&s_tim5, s_tim5.Init.Period);
    __HAL_TIM_SET_COUNTER(&s_tim5, s_tim5.Init.Period);
    HAL_TIM_PWM_Start_DMA(&s_tim5, TIM_CHANNEL_3, (uint32_t*)data, size);
}

void hal_tim_neoled_set_reset()
{
    __HAL_TIM_SET_COMPARE(&s_tim5, TIM_CHANNEL_3, 0);
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

extern "C"
{
    void TIM7_IRQHandler()
    {
        if (__HAL_TIM_GET_FLAG(&s_tim7, TIM_FLAG_UPDATE) != RESET &&
            __HAL_TIM_GET_IT_SOURCE(&s_tim7, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&s_tim7, TIM_IT_UPDATE);
            __HAL_TIM_CLEAR_FLAG(&s_tim7, TIM_FLAG_UPDATE);
            s_timer_us_cnt += 65536;
        }
    }

    void TIM5_IRQHandler()
    {
        if (__HAL_TIM_GET_FLAG(&s_tim5, TIM_FLAG_UPDATE) != RESET &&
            __HAL_TIM_GET_IT_SOURCE(&s_tim5, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&s_tim5, TIM_IT_UPDATE);
            __HAL_TIM_CLEAR_FLAG(&s_tim5, TIM_FLAG_UPDATE);
            __HAL_TIM_DISABLE_IT(&s_tim5, TIM_IT_UPDATE);
            HAL_TIM_PWM_Stop(&s_tim5, TIM_CHANNEL_3);

            if (s_timer_neoled_update_cb != nullptr)
            {
                s_timer_neoled_update_cb(s_neoled_param);
            }
        }
    }

    void DMA2_Channel2_IRQHandler(void)
    {
        HAL_DMA_IRQHandler(&s_hdma_tim5_ch3_up);
    }

    void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef*)
    {
        HAL_TIM_PWM_Stop_DMA(&s_tim5, TIM_CHANNEL_3);

        __HAL_TIM_SET_COMPARE(&s_tim5, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COUNTER(&s_tim5, 0);

        // Configure the timer to make a delay of > 80uS
        s_tim5.Init.Period = s_timer_cnt_frequency * 0.0005f;   // 500us (no need faster...)
        __HAL_TIM_SET_AUTORELOAD(&s_tim5, s_tim5.Init.Period);
        __HAL_TIM_ENABLE_IT(&s_tim5, TIM_IT_UPDATE);

        HAL_TIM_PWM_Start(&s_tim5, TIM_CHANNEL_3);
    }

    void HAL_IncTick(void)
    {
        s_timer_ms_cnt++;
    }

    uint32_t HAL_GetTick(void)
    {
        return s_timer_ms_cnt;
    }
}
