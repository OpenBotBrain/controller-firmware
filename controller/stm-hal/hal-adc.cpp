#include <assert.h>
#include <config/appconfig.h>
#include <cstring>
#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-datatype.hpp>

struct AdcData
{
    FinishAdcCb cb;
    void* param;
    uint8_t sample_offset;
};

static const ADCChannelConfig* s_adc_config = nullptr;
static ADC_HandleTypeDef s_adc1;
static ADC_HandleTypeDef s_adc3;
static DMA_HandleTypeDef s_hdma_adc1;
static DMA_HandleTypeDef s_hdma_adc3;
static uint16_t s_adc_sample[ADC_CHANNEL_TOTAL] = {0};
static AdcData s_adc_data[ADC_CHANNEL_TOTAL] = {0};

static constexpr uint32_t s_adc_rank[] = {
    ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2, ADC_REGULAR_RANK_3, ADC_REGULAR_RANK_4,
    ADC_REGULAR_RANK_5, ADC_REGULAR_RANK_6, ADC_REGULAR_RANK_7, ADC_REGULAR_RANK_8,
    ADC_REGULAR_RANK_9, ADC_REGULAR_RANK_10, ADC_REGULAR_RANK_11, ADC_REGULAR_RANK_12,
    ADC_REGULAR_RANK_13, ADC_REGULAR_RANK_14, ADC_REGULAR_RANK_15, ADC_REGULAR_RANK_16
};

static void finish_sample_callback_update(AdcType type)
{
    for (int i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        AdcData* data = &s_adc_data[i];
        if (data->cb != nullptr && type == s_adc_config[i].adc)
        {
            data->cb(s_adc_sample[data->sample_offset], data->param);
        }
    }
}

static uint8_t get_adc_sample_offset_from_type(const uint8_t id)
{
    for (uint8_t i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        if (s_adc_config[i].id == id)
        {
            return i;
        }
    }
    assert(0);
    return 0;
}

void hal_adc_init_default(const BoardSpecificConfig* board_config)
{
    s_adc_config = board_config->adc_config;
    assert(s_adc_config);

    std::memset(static_cast<void*>(&s_adc_data[0]), 0, sizeof(s_adc_data));

    // Configure ADC - no ADC sequence - continuous conversion mode -> ADC in independent mode
    s_adc1.Instance = ADC1;
    s_adc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
    s_adc1.Init.Resolution = ADC_RESOLUTION_12B;
    s_adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    s_adc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    s_adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    s_adc1.Init.LowPowerAutoWait = DISABLE;
    s_adc1.Init.ContinuousConvMode = DISABLE;
    s_adc1.Init.NbrOfConversion = board_config->total_adc1;
    s_adc1.Init.DiscontinuousConvMode = ENABLE;
    s_adc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
    s_adc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    s_adc1.Init.DMAContinuousRequests = ENABLE;
    s_adc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    s_adc1.Init.OversamplingMode = DISABLE;
    assert (HAL_ADC_Init(&s_adc1) == HAL_OK);

    s_adc3 = s_adc1;
    s_adc3.Instance = ADC3;
    s_adc3.Init.NbrOfConversion = board_config->total_adc3;
    assert (HAL_ADC_Init(&s_adc3) == HAL_OK);

    // Configure DMA
    s_hdma_adc1.Instance = DMA1_Channel1;
    s_hdma_adc1.Init.Request = DMA_REQUEST_0;
    s_hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    s_hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    s_hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    s_hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    s_hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    s_hdma_adc1.Init.Mode = DMA_CIRCULAR;
    s_hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    assert (HAL_DMA_Init(&s_hdma_adc1) == HAL_OK);

    __HAL_LINKDMA(&s_adc1, DMA_Handle, s_hdma_adc1);

    s_hdma_adc3 = s_hdma_adc1;
    s_hdma_adc3.Instance = DMA2_Channel5;
    s_hdma_adc3.Init.Request = DMA_REQUEST_0;
    assert (HAL_DMA_Init(&s_hdma_adc3) == HAL_OK);

    __HAL_LINKDMA(&s_adc3, DMA_Handle, s_hdma_adc3);

    // Configure ADC Channels
    ADC_MultiModeTypeDef multimode = {0};

    multimode.Mode = ADC_MODE_INDEPENDENT;

    assert (HAL_ADCEx_MultiModeConfigChannel(&s_adc1, &multimode) == HAL_OK);
    assert (HAL_ADCEx_MultiModeConfigChannel(&s_adc3, &multimode) == HAL_OK);

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;

    int adc1_offset = 0;
    int adc3_offset = 0;
    for (uint8_t i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        ADC_HandleTypeDef* adc;
        const ADCChannelConfig* config = &s_adc_config[i];
        if (config->adc == AdcType::TypeADC1)
        {
            adc = &s_adc1;
            sConfig.Rank = s_adc_rank[adc1_offset];
            adc1_offset++;
        }
        else
        {
            adc = &s_adc3;
            sConfig.Rank = s_adc_rank[adc3_offset];
            adc3_offset++;
        }
        sConfig.Channel = config->adc_channel;
        assert (HAL_ADC_ConfigChannel(adc, &sConfig) == HAL_OK);
    }

    HAL_ADCEx_Calibration_Start(&s_adc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&s_adc3, ADC_SINGLE_ENDED);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, PRI_HARD_ADC, 0);
    HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, PRI_HARD_ADC, 0);

    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);

    __HAL_DMA_ENABLE_IT(&s_hdma_adc1, DMA_IT_TC);
    __HAL_DMA_ENABLE_IT(&s_hdma_adc3, DMA_IT_TC);

    // Start ADC with DMA
    HAL_ADC_Start_DMA(&s_adc1, (uint32_t*)&s_adc_sample[0], board_config->total_adc1);
    HAL_ADC_Start_DMA(&s_adc3, (uint32_t*)&s_adc_sample[board_config->total_adc1], board_config->total_adc3);
}

void hal_adc_init_channel(const uint8_t id, FinishAdcCb cb, void* param)
{
    assert(id < ADC_CHANNEL_TOTAL);

    s_adc_data[id].cb = cb;
    s_adc_data[id].param = param;
    s_adc_data[id].sample_offset = get_adc_sample_offset_from_type(id);
}

extern "C" {

void DMA1_Channel1_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&s_hdma_adc1, DMA_FLAG_TC4) != RESET)
    {
        finish_sample_callback_update(AdcType::TypeADC1);
    }
    else if (__HAL_DMA_GET_FLAG(&s_hdma_adc1, DMA_FLAG_HT4) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&s_hdma_adc1, DMA_FLAG_HT4);
    }
}

void DMA2_Channel5_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&s_hdma_adc3, DMA_FLAG_TC4) != RESET)
    {
        finish_sample_callback_update(AdcType::TypeADC3);
    }
    else if (__HAL_DMA_GET_FLAG(&s_hdma_adc3, DMA_FLAG_HT4) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&s_hdma_adc3, DMA_FLAG_HT4);
    }
}

}

uint32_t hal_adc_get_timer_sample_frequency()
{
    return ADC_CHANNEL_TOTAL * ADC_SAMPLE_FREQUENCY_HZ;
}