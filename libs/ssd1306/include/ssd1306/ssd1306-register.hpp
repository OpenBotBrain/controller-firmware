#pragma once

#include <cstdint>

namespace SSD1306_Register
{
    enum SSD1306_Reg : uint8_t
    {
        SSD1306_MEMORYMODE = 0x20,
        SSD1306_COLUMNADDR = 0x21,
        SSD1306_PAGEADDR = 0x22,
        SSD1306_SETCONTRAST = 0x81,
        SSD1306_CHARGEPUMP = 0x8D,
        SSD1306_SEGREMAP = 0xA0,
        SSD1306_DISPLAYALLON_RESUME = 0xA4,
        SSD1306_DISPLAYALLON = 0xA5,
        SSD1306_NORMALDISPLAY = 0xA6,
        SSD1306_INVERTDISPLAY = 0xA7,
        SSD1306_SETMULTIPLEX = 0xA8,
        SSD1306_DISPLAYOFF = 0xAE,
        SSD1306_DISPLAYON = 0xAF,
        SSD1306_COMSCANINC = 0xC0,
        SSD1306_COMSCANDEC = 0xC8,
        SSD1306_SETDISPLAYOFFSET = 0xD3,
        SSD1306_SETDISPLAYCLOCKDIV = 0xD5,
        SSD1306_SETPRECHARGE = 0xD9,
        SSD1306_SETCOMPINS = 0xDA,
        SSD1306_SETVCOMDETECT = 0xDB,

        SSD1306_SETSTARTLINE = 0x40,

        SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26,              // Init rt scroll
        SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27,               // Init left scroll
        SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29, // Init diag scroll
        SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A,  // Init diag scroll
        SSD1306_DEACTIVATE_SCROLL = 0x2E,                    // Stop scroll
        SSD1306_ACTIVATE_SCROLL = 0x2F,                      // Start scroll
        SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3,             // Set scroll range
    };

    enum SSD1306_Address : uint8_t
    {
        ADDRESS_32BIT_DISPALY = (0x3C << 1),   // hight is 32 lines
        ADDRESS_NORMAL = (0x3D << 1)
    };

}