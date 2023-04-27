#include <assert.h>
#include <config/appconfig.h>
#include <cstring>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-adc.hpp>

enum class AdcType : uint8_t
{
    TypeADC1 = 0,
    TypeADC3
};

struct AdcChannelConfig
{
    uint8_t id;
    AdcType adc;
    uint32_t adc_channel;
};

static constexpr AdcChannelConfig s_adc_config[ADC_CHANNEL_TOTAL] =
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

static constexpr uint8_t get_total_channels(AdcType type)
{
    uint8_t total = 0;
    for (int i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        if (s_adc_config[i].adc == type)
        {
            total++;
        }
    }
    return total;
}

static uint16_t s_adc_sample[ADC_CHANNEL_TOTAL] = {0};
static ADC_HandleTypeDef s_adc1;
static ADC_HandleTypeDef s_adc3;
static DMA_HandleTypeDef s_hdma_adc1;
static DMA_HandleTypeDef s_hdma_adc3;
static uint8_t s_total_channel_adc1 = get_total_channels(AdcType::TypeADC1);
static uint8_t s_total_channel_adc3 = get_total_channels(AdcType::TypeADC3);

struct AdcData
{
    FinishAdcCb cb;
    void* param;
    uint8_t sample_offset;  // position where the sample is on the main ADC buffer
};

static AdcData s_adc_data[ADC_CHANNEL_TOTAL] = {0};

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

void hal_adc_init_channel(const uint8_t id, FinishAdcCb cb, void* param)
{
    assert(id < ADC_CHANNEL_TOTAL);

    s_adc_data[id].cb = cb;
    s_adc_data[id].param = param;
    s_adc_data[id].sample_offset = get_adc_sample_offset_from_type(id);
}

void hal_adc_init_default(uint8_t board_rev)
{
    (void) board_rev;

    std::memset(static_cast<void*>(&s_adc_data[0]), 0, sizeof(s_adc_data));

    // Configure ADC
    s_adc1.Instance = ADC1;
    s_adc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
    s_adc1.Init.Resolution = ADC_RESOLUTION_12B;
    s_adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    s_adc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    s_adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    s_adc1.Init.LowPowerAutoWait = DISABLE;
    s_adc1.Init.ContinuousConvMode = DISABLE;
    s_adc1.Init.NbrOfConversion = s_total_channel_adc1;
    s_adc1.Init.DiscontinuousConvMode = DISABLE;
    s_adc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
    s_adc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    s_adc1.Init.DMAContinuousRequests = ENABLE;
    s_adc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    s_adc1.Init.OversamplingMode = DISABLE;

    s_adc3 = s_adc1;
    s_adc3.Instance = ADC3;
    s_adc3.Init.NbrOfConversion = s_total_channel_adc3;
    assert (HAL_ADC_Init(&s_adc1) == HAL_OK);
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
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;

    for (uint8_t i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        ADC_HandleTypeDef* adc;
        if (s_adc_config[i].adc == AdcType::TypeADC1)
        {
            adc = &s_adc1;
        }
        else
        {
            adc = &s_adc3;
        }
        sConfig.Channel = s_adc_config[i].adc_channel;
        assert (HAL_ADC_ConfigChannel(adc, &sConfig) == HAL_OK);
    }

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
