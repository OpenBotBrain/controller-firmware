#include <assert.h>
#include <ssd1306/ssd1306.hpp>

SSD1306::SSD1306(const SSD1306_Config& config) : m_config(config)
{
    m_width = config.display_width;
    m_height= config.display_height;
}

void SSD1306::send_command(const uint8_t* tx, uint8_t size)
{
    assert(sizeof(m_aux_buffer) >= size);
    std::memcpy(m_aux_buffer, tx, size);

    if (m_config.i2c_transaction_cb)
    {
        // 0bXX000000 -> Co = 0, D/C = 0
        m_config.i2c_transaction_cb(m_config.i2c_address, 0x00, m_aux_buffer, size);
    }
    else if (m_config.spi_transaction_cb)
    {
        m_config.set_reset_dc(false);   // set to cmd mode
        m_config.set_reset_cs(false);
        m_config.spi_transaction_cb(m_aux_buffer, m_aux_buffer, size);
        m_config.set_reset_cs(true);
    }
}

void SSD1306::send_data(uint8_t* tx, uint8_t size)
{
    if (m_config.i2c_transaction_cb)
    {
        m_config.i2c_transaction_cb(m_config.i2c_address, 0x40, tx, size);
    }
    else if (m_config.spi_transaction_cb)
    {
        m_config.set_reset_dc(true);   // set to cmd mode
        m_config.set_reset_cs(false);
        m_config.spi_transaction_cb(tx, nullptr, size);
        m_config.set_reset_cs(true);
    }
}

// PUBLIC API
void SSD1306::init(DelayCB delaycb)
{
    static constexpr uint8_t s_init_1[] = {
        SSD1306_Register::SSD1306_Reg::SSD1306_DISPLAYOFF,          // 0xAE
        SSD1306_Register::SSD1306_Reg::SSD1306_SETDISPLAYCLOCKDIV,  // 0xD5
        0x80,                                                       // the suggested ratio 0x80
        SSD1306_Register::SSD1306_Reg::SSD1306_SETMULTIPLEX };      // 0xA8

    static constexpr uint8_t s_init_2[] = {
        SSD1306_Register::SSD1306_Reg::SSD1306_SETDISPLAYOFFSET,    // 0xD3
        0x00,                                                       // 0x0 act like ks0108
        SSD1306_Register::SSD1306_Reg::SSD1306_SETSTARTLINE | 0x0,
        SSD1306_Register::SSD1306_Reg::SSD1306_CHARGEPUMP };

    static constexpr uint8_t s_init_3[] = {
        SSD1306_Register::SSD1306_Reg::SSD1306_MEMORYMODE,          // 0x20
        0x00,                                                       // 0x0 act like ks0108
        SSD1306_Register::SSD1306_Reg::SSD1306_SEGREMAP | 0x1,
        SSD1306_Register::SSD1306_Reg::SSD1306_COMSCANDEC };

    static constexpr uint8_t s_init_5[] = {
        SSD1306_Register::SSD1306_Reg::SSD1306_SETVCOMDETECT,       // 0xDB
        0x40,
        SSD1306_Register::SSD1306_Reg::SSD1306_DISPLAYALLON_RESUME, // 0xA4
        SSD1306_Register::SSD1306_Reg::SSD1306_NORMALDISPLAY,       // 0xA6
        SSD1306_Register::SSD1306_Reg::SSD1306_DEACTIVATE_SCROLL,
        SSD1306_Register::SSD1306_Reg::SSD1306_DISPLAYON };         // Main screen turn on

    // Make sure buffer has the right size
    assert(SSD1306::get_buffer_size(m_config.display_width, m_config.display_height) == m_config.buffer_size);

    clearDisplay();

    if (m_config.i2c_transaction_cb)
    {
        // Make sure spi is not in used
        assert(m_config.spi_transaction_cb == nullptr);
    }

    if (m_config.spi_transaction_cb)
    {
        // make sure i2c is not in used and addition spi cb are set
        assert(m_config.i2c_transaction_cb == nullptr);
        assert(m_config.set_reset_dc);
        assert(m_config.set_reset_cs);
        m_config.set_reset_cs(true);
        m_config.set_reset_dc(false);   // set to cmd mode
    }

    // Reset SSD1306 if requested and reset pin specified in constructor
    if (m_config.set_reset_reset_line)
    {
        assert(delaycb);
        m_config.set_reset_reset_line(true);
        delaycb(1);                   // VDD goes high at start, pause for 1 ms
        m_config.set_reset_reset_line(false); // Bring reset low
        delaycb(10);                  // Wait 10 ms
        m_config.set_reset_reset_line(true);  // Bring out of reset
    }

    uint8_t com_pins = 0x02;
    uint8_t pre_charge = m_config.external_vcc ? 0x22 : 0xF1;
    m_contrast = 0x8F;

    if ((m_config.display_width == 128) && (m_config.display_height == 32))
    {
        com_pins = 0x02;
        m_contrast = 0x8F;
    }
    else if ((m_config.display_width == 128) && (m_config.display_height == 64))
    {
        com_pins = 0x12;
        m_contrast = m_config.external_vcc ? 0x9F : 0xCF;
    }
    else if ((m_config.display_width == 96) && (m_config.display_height == 16))
    {
        com_pins = 0x2; // ada x12
        m_contrast = m_config.external_vcc ? 0x10 : 0xAF;
    }
    else
    {
        // Other screen varieties -- TODO
    }

    uint8_t init_4[] = {
        SSD1306_Register::SSD1306_Reg::SSD1306_SETCOMPINS,
        com_pins,
        SSD1306_Register::SSD1306_Reg::SSD1306_SETCONTRAST,
        m_contrast,
        SSD1306_Register::SSD1306_Reg::SSD1306_SETPRECHARGE,
        pre_charge
    };

    // Initialize module parametes
    send_command(s_init_1, sizeof(s_init_1));

    uint8_t data = m_config.display_height - 1;
    send_command(&data, 1);

    send_command(s_init_2, sizeof(s_init_2));

    data = m_config.external_vcc ? 0x10 : 0x14;
    send_command(&data, 1);

    send_command(s_init_3, sizeof(s_init_3));
    for (size_t i = 0; i < sizeof(init_4); i++)
    {
        send_command(init_4 + i, 1);    // one at the time
    }
    send_command(s_init_5, sizeof(s_init_5));
}

void SSD1306::update()
{
    static constexpr uint8_t s_instruction[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_PAGEADDR,
        0,                                                  // Page start address
        0xFF,                                               // Page end (not really, but works here)
        SSD1306_Register::SSD1306_Reg::SSD1306_COLUMNADDR,
        0
    }; // Column start address

    send_command(s_instruction, sizeof(s_instruction));
    uint8_t data = m_config.display_width - 1;  // Column end address
    send_command(&data, 1);

    uint8_t *ptr = m_config.buffer;
    uint16_t count = m_config.buffer_size;

    while (count)
    {
        uint8_t tran_size = count > MAX_BYTE_SIZE ? MAX_BYTE_SIZE : count;
        send_data(ptr, tran_size);
        ptr += tran_size;
        count -= tran_size;
    }
}

void SSD1306::drawPixel(int16_t x, int16_t y, Color color)
{
    if ((x >= 0) && (x < m_config.display_width) && (y >= 0) && (y < m_config.display_height))
    {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch (m_rotation)
        {
            case 1:
                swap(x, y);
                x = m_config.display_width - x - 1;
                break;
            case 2:
                x = m_config.display_width - x - 1;
                y = m_config.display_height - y - 1;
                break;
            case 3:
                swap(x, y);
                y = m_config.display_height - y - 1;
                break;
        }

        switch (color)
        {
            case Color::WHITE:
                m_config.buffer[x + (y / 8) * m_config.display_width] |= (1 << (y & 7));
                break;
            case Color::BLACK:
                m_config.buffer[x + (y / 8) * m_config.display_width] &= ~(1 << (y & 7));
                break;
            case Color::INVERSE:
                m_config.buffer[x + (y / 8) * m_config.display_width] ^= (1 << (y & 7));
            break;
        }
    }
}

void SSD1306::clearDisplay()
{
    std::memset(m_config.buffer, 0, m_config.buffer_size);
}

void SSD1306::drawFastHLine(int16_t x, int16_t y, int16_t w, Color color)
{
    bool bSwap = false;
    switch (m_rotation)
    {
        case 1:
            // 90 degree rotation, swap x & y for rotation, then invert x
            bSwap = true;
            swap(x, y);
            x = m_config.display_width - x - 1;
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = m_config.display_width - x - 1;
            y = m_config.display_height - y - 1;
            x -= (w - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation,
            // then invert y and adjust y for w (not to become h)
            bSwap = true;
            swap(x, y);
            y = m_config.display_height - y - 1;
            y -= (w - 1);
            break;
    }

    if (bSwap)
    {
        drawFastVLineInternal(x, y, w, color);
    }
    else
    {
        drawFastHLineInternal(x, y, w, color);
    }
}

void SSD1306::drawFastVLine(int16_t x, int16_t y, int16_t h, Color color)
{
    bool bSwap = false;
    switch (m_rotation)
    {
        case 1:
            // 90 degree rotation, swap x & y for rotation,
            // then invert x and adjust x for h (now to become w)
            bSwap = true;
            swap(x, y);
            x = m_config.display_width - x - 1;
            x -= (h - 1);
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = m_config.display_width - x - 1;
            y = m_config.display_height - y - 1;
            y -= (h - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation, then invert y
            bSwap = true;
            swap(x, y);
            y = m_config.display_height - y - 1;
            break;
    }

    if (bSwap)
    {
        drawFastHLineInternal(x, y, h, color);
    }
    else
    {
        drawFastVLineInternal(x, y, h, color);
    }
}

void SSD1306::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, Color color)
{
    if ((y >= 0) && (y < m_config.display_height))
    {
        // Y coord in bounds?
        if (x < 0)
        {
            // Clip left
            w += x;
            x = 0;
        }
        if ((x + w) > m_config.display_width)
        {
            // Clip right
            w = (m_config.display_width - x);
        }
        if (w > 0)
        {
            // Proceed only if width is positive
            uint8_t* pBuf = &m_config.buffer[(y / 8) * m_config.display_width + x];
            uint8_t mask = 1 << (y & 7);
            switch (color)
            {
                case Color::WHITE:
                    while (w--)
                    {
                        *pBuf++ |= mask;
                    };
                    break;
                case Color::BLACK:
                    mask = ~mask;
                    while (w--)
                    {
                        *pBuf++ &= mask;
                    };
                    break;
                case Color::INVERSE:
                    while (w--)
                    {
                        *pBuf++ ^= mask;
                    };
                    break;
            }
        }
    }
}

void SSD1306::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, Color color)
{
    if ((x >= 0) && (x < m_config.display_width))
    {
        // X coord in bounds?
        if (__y < 0)
        {
            // Clip top
            __h += __y;
            __y = 0;
        }
        if ((__y + __h) > m_config.display_height)
        {
            // Clip bottom
            __h = (m_config.display_height - __y);
        }
        if (__h > 0)
        {
            // Proceed only if height is now positive
            // this display doesn't need ints for coordinates,
            // use local byte registers for faster juggling
            uint8_t y = __y, h = __h;
            uint8_t *pBuf = &m_config.buffer[(y / 8) * m_config.display_width + x];

            // do the first partial byte, if necessary - this requires some masking
            uint8_t mod = (y & 7);
            if (mod)
            {
                // mask off the high n bits we want to set
                mod = 8 - mod;
                // note - lookup table results in a nearly 10% performance
                // improvement in fill* functions
                // uint8_t mask = ~(0xFF >> mod);
                static constexpr uint8_t premask[8] = {
                    0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE};
                uint8_t mask = premask[mod];

                // adjust the mask if we're not going to reach the end of this byte
                if (h < mod)
                {
                    mask &= (0XFF >> (mod - h));
                }

                switch (color)
                {
                    case Color::WHITE:
                        *pBuf |= mask;
                        break;
                    case Color::BLACK:
                        *pBuf &= ~mask;
                        break;
                    case Color::INVERSE:
                        *pBuf ^= mask;
                        break;
                }
                pBuf += m_config.display_width;
            }

            if (h >= mod)
            {
                // More to go?
                h -= mod;
                // Write solid bytes while we can - effectively 8 rows at a time
                if (h >= 8)
                {
                    if (color == Color::INVERSE)
                    {
                        // separate copy of the code so we don't impact performance of
                        // black/white write version with an extra comparison per loop
                        do {
                            *pBuf ^= 0xFF; // Invert byte
                            pBuf += m_config.display_width; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    }
                    else
                    {
                        // store a local value to work with
                        uint8_t val = (color != Color::BLACK) ? 255 : 0;
                        do {
                            *pBuf = val;   // Set byte
                            pBuf += m_config.display_width; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    }
                }

                if (h)
                {
                    // Do the final partial byte, if necessary
                    mod = h & 7;
                    // this time we want to mask the low bits of the byte,
                    // vs the high bits we did above
                    // uint8_t mask = (1 << mod) - 1;
                    // note - lookup table results in a nearly 10% performance
                    // improvement in fill* functions
                    static constexpr uint8_t postmask[8] = {
                        0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
                    uint8_t mask = postmask[mod];
                    switch (color)
                    {
                        case Color::WHITE:
                            *pBuf |= mask;
                            break;
                        case Color::BLACK:
                            *pBuf &= ~mask;
                            break;
                        case Color::INVERSE:
                            *pBuf ^= mask;
                            break;
                    }
                }
            }
        }
    }
}

bool SSD1306::getPixel(int16_t x, int16_t y)
{
    if ((x >= 0) && (x < m_config.display_width) && (y >= 0) && (y < m_config.display_height))
    {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch (m_rotation)
        {
            case 1:
                swap(x, y);
                x = m_config.display_width - x - 1;
                break;
            case 2:
                x = m_config.display_width - x - 1;
                y = m_config.display_height - y - 1;
                break;
            case 3:
                swap(x, y);
                y = m_config.display_height - y - 1;
                break;
            }
        return (m_config.buffer[x + (y / 8) * m_config.display_width] & (1 << (y & 7)));
    }
    return false; // Pixel out of bounds
}

uint8_t* SSD1306::getBuffer(void)
{
    return m_config.buffer;
}

void SSD1306::startscrollright(uint8_t start, uint8_t stop)
{
    static constexpr uint8_t scrollList1a[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_RIGHT_HORIZONTAL_SCROLL, 0X00
    };
    send_command(scrollList1a, sizeof(scrollList1a));\

    uint8_t data[] = {start, 0, stop};
    send_command(data, sizeof(data));

    static constexpr uint8_t scrollList1b[] =
    {
        0X00, 0XFF, SSD1306_Register::SSD1306_Reg::SSD1306_ACTIVATE_SCROLL
    };
    send_command(scrollList1b, sizeof(scrollList1b));
}

void SSD1306::startscrollleft(uint8_t start, uint8_t stop)
{
    static constexpr uint8_t scrollList2a[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_LEFT_HORIZONTAL_SCROLL, 0X00
    };
    send_command(scrollList2a, sizeof(scrollList2a));

    uint8_t data[] = {start, 0, stop};
    send_command(data, sizeof(data));

    static constexpr uint8_t scrollList2b[] =
    {
        0X00, 0XFF, SSD1306_Register::SSD1306_Reg::SSD1306_ACTIVATE_SCROLL
    };
    send_command(scrollList2b, sizeof(scrollList2b));
}

void SSD1306::startscrolldiagright(uint8_t start, uint8_t stop)
{
    static constexpr uint8_t scrollList3a[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00
    };
    send_command(scrollList3a, sizeof(scrollList3a));
    send_command(&m_config.display_height);

    static constexpr uint8_t scrollList3b[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, 0X00
    };
    send_command(scrollList3b, sizeof(scrollList3b));

    uint8_t data[] = {start, 0, stop};
    send_command(data, sizeof(data));

    static constexpr uint8_t scrollList3c[] =
    {
        0X01, SSD1306_Register::SSD1306_Reg::SSD1306_ACTIVATE_SCROLL
    };
    send_command(scrollList3c, sizeof(scrollList3c));
}

void SSD1306::startscrolldiagleft(uint8_t start, uint8_t stop)
{
    static constexpr uint8_t scrollList4a[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00
    };
    send_command(scrollList4a, sizeof(scrollList4a));
    send_command(&m_config.display_height);

    static constexpr uint8_t scrollList4b[] =
    {
        SSD1306_Register::SSD1306_Reg::SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0X00
    };
    send_command(scrollList4b, sizeof(scrollList4b));

    uint8_t data[] = {start, 0, stop};
    send_command(data, sizeof(data));

    static constexpr uint8_t scrollList4c[] =
    {
        0X01, SSD1306_Register::SSD1306_Reg::SSD1306_ACTIVATE_SCROLL
    };
    send_command(scrollList4c, sizeof(scrollList4c));
}

void SSD1306::stopscroll(void)
{
    uint8_t data = SSD1306_Register::SSD1306_Reg::SSD1306_DEACTIVATE_SCROLL;
    send_command(&data, 1);
}

