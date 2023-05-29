#include <stm-hal/hal-adc.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <gscope/gscope.hpp>

enum AdcSampleType
{
    ADC_TYPE_3V3 = 0,
    ADC_TYPE_5V,
    ADC_TYPE_9V,
    ADC_TYPE_VBUS,
    ADC_TYPE_VBAT,
    ADC_TYPE_TOTAL
};

static uint16_t s_raw_adc[ADC_TYPE_TOTAL];
static float s_rail_voltage[ADC_TYPE_TOTAL];
static uint32_t s_timestamp;

GScopeChannel(s_voltage_debug, "voltage", float, ADC_TYPE_TOTAL)

static void s_3v3_sample(uint16_t value, void*)
{
    s_raw_adc[ADC_TYPE_3V3] = value;
}

static void s_5v_sample(uint16_t value, void*)
{
    s_raw_adc[ADC_TYPE_5V] = value;
}

static void s_9v_sample(uint16_t value, void*)
{
    s_raw_adc[ADC_TYPE_9V] = value;
}

static void s_vbus_sample(uint16_t value, void*)
{
    s_raw_adc[ADC_TYPE_VBUS] = value;
}

static void s_vbat_sample(uint16_t value, void*)
{
    s_raw_adc[ADC_TYPE_VBAT] = value;
}

void system_status_enable(bool enable)
{
    if (enable)
    {
        hal_gpio_set_pin(VBAT_ADC_ON_IO);
    }
    else
    {
        hal_gpio_reset_pin(VBAT_ADC_ON_IO);
    }
}

void system_status_init()
{
    hal_adc_init_channel(ADC_CHANNEL_TYPE_3V3_SENSE, s_3v3_sample, nullptr);
    hal_adc_init_channel(ADC_CHANNEL_TYPE_5V_SENSE, s_5v_sample, nullptr);
    hal_adc_init_channel(ADC_CHANNEL_TYPE_9V_SENSE, s_9v_sample, nullptr);
    hal_adc_init_channel(ADC_CHANNEL_TYPE_VBUS_SENSE, s_vbus_sample, nullptr);
    hal_adc_init_channel(ADC_CHANNEL_TYPE_VBAT_SENSE, s_vbat_sample, nullptr);

    system_status_enable(true);     // TODO: THIS SHOULD COME FROM ON / OFF ROUTINE!
}

void system_status_update()
{
    s_rail_voltage[ADC_TYPE_3V3] = static_cast<float>(s_raw_adc[ADC_TYPE_3V3]) / 1000.73f;
    s_rail_voltage[ADC_TYPE_5V] = static_cast<float>(s_raw_adc[ADC_TYPE_5V]) / 620.45f;
    s_rail_voltage[ADC_TYPE_9V] = static_cast<float>(s_raw_adc[ADC_TYPE_9V]) / 314.9515f;
    s_rail_voltage[ADC_TYPE_VBUS] = static_cast<float>(s_raw_adc[ADC_TYPE_VBUS]) / 203.4277;
    s_rail_voltage[ADC_TYPE_VBAT] = static_cast<float>(s_raw_adc[ADC_TYPE_VBAT]) / 818.27f;

    uint32_t now = hal_timer_32_ms();

    if ((now - s_timestamp) >= 50)
    {
        s_timestamp = now;
        s_voltage_debug.produce(s_rail_voltage);
    }
}
