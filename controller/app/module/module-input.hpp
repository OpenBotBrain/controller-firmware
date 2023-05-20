#pragma once

class ModuleInput
{
    public:
        struct Config
        {
            uint16_t pin1_current_enable_io;
            uint16_t pin2_detect_io;
            uint16_t pin5_io;
            uint16_t pin6_io;
            uint16_t pin6_enable_n_io;
            uint8_t pin1_adc_channel;
            uint8_t pin6_adc_channel;
            uint8_t pin56_mode[4]; // input/output/serial/i2c
        };

        enum class ModeConfiguration : uint8_t
        {
            ANALOG = 0,
            INPUT,
            OUTPUT,
            SERIAL,
            I2C,
        };

        enum class PinID
        {
            PIN1 = 0,
            PIN2,
            PIN5,
            PIN6
        };

        ModuleInput(const Config& config) : m_config(config) {}

        void init();
        void update();

        float get_voltage(PinID id);
        bool get_gpio(PinID id);

        void set_mode(PinID id, ModeConfiguration mode);
        void set_gpio(PinID id, bool enable);
        void set_9v_output(bool enable);
        void uart_write(const uint8_t* data, uint16_t size);
        uint16_t uart_read(uint8_t* read_buffer, uint16_t buffer_size);
        bool i2c_read(uint8_t address, uint8_t reg, uint16_t data_size);
        bool i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint16_t buf_size);

    private:
        const Config& m_config;
};