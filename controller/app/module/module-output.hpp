#pragma once

#include <cstdint>

class OutputPort
{
    public:
        struct Config
        {
            uint16_t pin5_adc_enable_n_io;
            uint16_t pin6_adc_enable_n_io;
            uint8_t encoder_id;
            uint8_t motor_pwm_id;
            uint16_t fault_io;
            uint8_t adc_pin5_channel;
            uint8_t adc_pin6_channel;
            bool invert_encoder_polarity;
        };

        OutputPort(const Config& config) : m_config(config) {}

        enum class Mode
        {
            DETECT_PIN = 0,     // TODO: NOT WORKING DUE TO HW ISSUE!
            ENCODER,
            ANALOG,
        };

        enum class PinID
        {
            PIN5 = 0,
            PIN6
        };

        void init();
        void update();

        uint16_t get_encoder_ticks();
        float get_voltage(PinID id);
        void set_mode(Mode mode);
        void set_pwm_duty(float duty);
        float get_pwm_duty();

    private:
        const Config& m_config;

        static void adc_new_sample_input5(uint16_t value, void* param);
        static void adc_new_sample_input6(uint16_t value, void* param);
        void convert_adc_reading_update();

        uint16_t m_adc_raw_reading_input5;
        uint16_t m_adc_raw_reading_input6;
        float m_pin5_voltage_v;
        float m_pin6_voltage_v;
        float m_pwm_value;
};