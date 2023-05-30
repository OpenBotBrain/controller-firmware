#pragma once

#include <cstdint>

class InputPort
{
    enum Pin56Mode
    {
        Pin56ModeInput = 0,
        Pin56ModeOutput,
        Pin56ModeSerial,
        Pin56ModeI2C
    };

    public:

        static constexpr uint32_t INVALID = 255;

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
            uint8_t uart_type;
            uint8_t i2c_type;
        };

        enum class ModeConfiguration : uint8_t
        {
            ANALOG = 0,     // Only PIN6 will work as ANALOG!
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

        using NewI2CData = void(*)(void*, uint8_t*, uint16_t);

        InputPort(const Config& config) : m_config(config) {}

        void init();
        void update();

        float get_voltage_v(PinID id);
        bool get_gpio(PinID id);

        void set_mode(ModeConfiguration mode);
        void set_gpio(PinID id, bool enable);
        void set_9v_output(bool enable);
        bool uart_write(const uint8_t* data, uint16_t size);
        bool uart_write_busy();
        uint16_t uart_read(uint8_t* read_buffer, uint16_t buffer_size);
        bool i2c_read(uint8_t address, uint8_t reg, uint16_t data_size, NewI2CData cb, void* param);
        bool i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint16_t buf_size);

    private:
        const Config& m_config;
        uint16_t m_adc_raw_reading_input1;
        uint16_t m_adc_raw_reading_input6;
        float m_pin1_voltage_v;
        float m_pin6_voltage_v;
        bool m_write_busy {false};
        bool m_r_enable;
        bool m_i2c_busy;
        NewI2CData m_i2c_rx_cb;
        void* m_i2c_param;

        void convert_adc_reading_update();
        void finish_i2c_transaction(bool, uint8_t*, uint16_t);

        static void uart_tx_end_callback(void* param);
        static void adc_new_sample_input1(uint16_t, void*);
        static void adc_new_sample_input6(uint16_t, void*);
        static void i2c_finish(bool, void*, uint8_t*, uint16_t);

        static constexpr float ADC_TO_VOLTAGE_INPUTS = 0.00127364196f;

};