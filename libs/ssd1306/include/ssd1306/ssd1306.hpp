#pragma once

#include <ssd1306/ssd1306-register.hpp>
#include <ssd1306/gscommon-graph.hpp>

class SSD1306 : public GSCommonGraph
{
    public:
        static constexpr uint8_t MAX_BYTE_SIZE = 32;

        using SetResetCB = void(*)(bool);
        // read, Address, register, data, data_size, callback, context
        using I2CTransactionCB = bool(*)(uint8_t, uint8_t, uint8_t*, uint16_t);

        // read, data tx, data rx, size
        using SPITransactionCB = bool(*)(const uint8_t*, uint8_t*, uint16_t);
        using DelayCB = void(*)(uint32_t);

        struct SSD1306_Config
        {
            uint8_t display_width;
            uint8_t display_height;
            uint8_t i2c_address;
            uint8_t* buffer;
            uint16_t buffer_size;
            SetResetCB set_reset_reset_line;    // reset line, null if is not used
            SetResetCB set_reset_cs;            // null if spi is not used
            SetResetCB set_reset_dc;            // null if spi is not used
            I2CTransactionCB i2c_transaction_cb;
            SPITransactionCB spi_transaction_cb;
            bool external_vcc;  // true if External display voltage source is used
        };

        SSD1306(const SSD1306_Config& config);

        // initialze display driver
        void init(DelayCB delaycb);

        // Refresh display

        // Main API
        virtual void update() override;
        virtual void drawPixel(int16_t x, int16_t y, Color color) override;
        virtual void clearDisplay() override;
        virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, Color color) override;
        virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, Color color) override;

        bool getPixel(int16_t x, int16_t y);
        uint8_t* getBuffer(void);
        void startscrollright(uint8_t start, uint8_t stop);
        void startscrollleft(uint8_t start, uint8_t stop);
        void startscrolldiagright(uint8_t start, uint8_t stop);
        void startscrolldiagleft(uint8_t start, uint8_t stop);
        void stopscroll(void);

        static constexpr int get_buffer_size(int w, int h)
        {
            return w * ((h + 7) / 8);
        }

    private:
        const SSD1306_Config& m_config;     // Main configuration struct
        uint8_t m_contrast;                 // normal contrast setting for this device
        uint8_t m_aux_buffer[6];

        void send_command(const uint8_t* tx, uint8_t size = 1);
        void send_data(uint8_t* tx, uint8_t size);

        void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, Color color);
        void drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, Color color);
};