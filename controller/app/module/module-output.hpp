#pragma once

class ModuleOutput
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
        };

        ModuleOutput(const Config& config) : m_config(config) {}

        enum class Mode
        {
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

    private:
        const Config& m_config;
};