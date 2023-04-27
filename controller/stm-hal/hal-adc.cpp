#include <assert.h>
#include <config/appconfig.h>
#include <cstring>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-adc.hpp>

enum class AdcType : uint8_t
{
    TypeADC1 = 0,
    TypeADC2,
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

static uint16_t s_adc_sample[ADC_CHANNEL_TOTAL] = {0};

struct AdcData
{
    FinishAdcCb cb;
    void* param;
    uint8_t sample_offset;  // position where the sample is on the main ADC buffer
};

static AdcData s_adc_data[ADC_CHANNEL_TOTAL] = {0};

static void finish_sample_callback_update()
{
    for (int i = 0; i < ADC_CHANNEL_TOTAL; i++)
    {
        AdcData* data = &s_adc_data[i];
        if (data->cb != nullptr)
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
    std::memset(static_cast<void*>(&s_adc_data[0]), 0, sizeof(s_adc_data));

    (void) board_rev;

    // ADC_MultiModeTypeDef multimode = {0};
    // ADC_ChannelConfTypeDef sConfig = {0};

    // /* USER CODE BEGIN ADC1_Init 1 */

    // /* USER CODE END ADC1_Init 1 */

    // /** Common config
    //  */
    // hadc1.Instance = ADC1;
    // hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
    // hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    // hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    // hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    // hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    // hadc1.Init.LowPowerAutoWait = DISABLE;
    // hadc1.Init.ContinuousConvMode = DISABLE;
    // hadc1.Init.NbrOfConversion = 1;
    // hadc1.Init.DiscontinuousConvMode = DISABLE;
    // hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
    // hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    // hadc1.Init.DMAContinuousRequests = DISABLE;
    // hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    // hadc1.Init.OversamplingMode = DISABLE;
    // if (HAL_ADC_Init(&hadc1) != HAL_OK)
    // {
    //     Error_Handler();
    // }


    (void) finish_sample_callback_update;
    (void) s_adc_data;
}
