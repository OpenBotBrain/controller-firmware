#pragma once

#include <stdint.h>
#include <cstring>
#include <cmath>
#include <ssd1306/gfxfont.hpp>

class GSCommonGraph
{
    public:
        enum class Color : uint8_t
        {
            BLACK = 0,    // Draw 'off' pixels
            WHITE,        // Draw 'on' pixels
            INVERSE       // Invert pixels
        };

        virtual void update() = 0;
        virtual void drawPixel(int16_t x, int16_t y, Color color) = 0;
        virtual void clearDisplay() = 0;
        virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, Color color) = 0;
        virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, Color color) = 0;

        uint8_t getRotation();
        uint8_t setRotation();
        uint8_t width();
        uint8_t height();
        void swap(int16_t& a, int16_t& b);

        void printText(const char* text)
        {
            while(*text != 0)
            {
                write(*text);
                text++;
            }
        }

        // --------------------------------------------------------------------------------
        //                  EXTENDED API
        // --------------------------------------------------------------------------------
        /**
         * @brief  Draw a PROGMEM-resident 1-bit image at the specified (x,y)
         *  position, using the specified foreground color (unset bits are transparent).
         *
         * @param x Top left corner x coordinate
         * @param y Top left corner y coordinate
         * @param bitmap byte array with monochrome bitmap
         * @param w Width of bitmap in pixels
         * @param h Height of bitmap in pixels
         * @param color Color of the line
         */
        void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, Color color);

        /**
         * @brief Write a line.  Bresenham's algorithm - thx wikpedia
         *
         * @param x0 Start point x coordinate
         * @param y0 Start point y coordinate
         * @param x1 End point x coordinate
         * @param y1 End point y coordinate
         * @param color Color of the line
         */
        void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color);

        /**
         * @brief Draw a line
         *
         * @param x0 Start point x coordinate
         * @param y0 Start point y coordinate
         * @param x1 End point x coordinate
         * @param y1 End point y coordinate
         * @param color Color of the line
         */
        void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color);

        /**
         * @brief Draw a rectangle with no fill color
         *
         * @param x Top left corner x coordinate
         * @param y Top left corner y coordinate
         * @param w Width in pixels
         * @param h Height in pixels
         * @param color Color of the line
         */
        void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color);

        /**
         * @brief Fill a rectangle completely with one color. Update in subclasses if desired!
         *
         * @param x Top left corner x coordinate
         * @param y Top left corner y coordinate
         * @param w Width in pixels
         * @param h Height in pixels
         * @param color Color of the line
         */
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color);

        /**
         * @brief Fill the framebuffer completely with one color
         *
         * @param color Color of the line
         */
        void fillScreen(Color color);

        /**
         * @brief Draw a circle outline
         *
         * @param x0 Center-point x coordinate
         * @param y0 Center-point y coordinate
         * @param r Radius of circle
         * @param color Color of the line
         */
        void drawCircle(int16_t x0, int16_t y0, int16_t r, Color color);

        /**
         * @brief Quarter-circle drawer, used to do circles and roundrects
         *
         * @param x0 Center-point x coordinate
         * @param y0 Center-point y coordinate
         * @param r Radius of circle
         * @param cornername Mask bit #1 or bit #2 to indicate which quarters of
         *  the circle we're doing
         * @param color Color of the line
         */
        void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, Color color);

        /**
         * @brief Draw a circle with filled color
         *
         * @param x0 Center-point x coordinate
         * @param y0 Center-point y coordinate
         * @param r Radius of circle
         * @param color Color of the line
         */
        void fillCircle(int16_t x0, int16_t y0, int16_t r, Color color);

        /**
         * @brief Quarter-circle drawer with fill, used for circles and roundrects
         *
         * @param x0 Center-point x coordinate
         * @param y0 Center-point y coordinate
         * @param r Radius of circle
         * @param corners Mask bits indicating which quarters we're doing
         * @param delta Offset from center-point, used for round-rects
         * @param color Color of the line
         */
        void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
            uint8_t corners, int16_t delta, Color color);

        /**
         * @brief Draw a rounded rectangle with no fill color
         *
         * @param x Top left corner x coordinate
         * @param y Top left corner y coordinate
         * @param w Width in pixels
         * @param h Height in pixels
         * @param r Radius of corner rounding
         * @param color Color of the line
         */
        void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color color);

        /**
         * @brief Draw a single character
         *
         * @param x Bottom left corner x coordinate
         * @param y Bottom left corner y coordinate
         * @param c The 8-bit font-indexed character (likely ascii)
         * @param color Color of the line
         * @param bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
         * @param size_x Font magnification level in X-axis, 1 is 'original' size
         * @param size_y ont magnification level in Y-axis, 1 is 'original' size
         */
        void drawChar(int16_t x, int16_t y, unsigned char c, Color color,
            Color bg, uint8_t size_x, uint8_t size_y);

        /**
         * @brief Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
         *
         * @param s Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
         */
        void setTextSize(uint8_t s);

        /**
         * @brief Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
         *
         * @param s_x Desired text width magnification level in X-axis. 1 is default
         * @param s_y Desired text width magnification level in Y-axis. 1 is default
         */
        void setTextSize(uint8_t s_x, uint8_t s_y);

        /**
         * @brief Set text font color with transparant background
         *
         * @param c Color of the line
         */
        void setTextColor(Color c);

        /**
         * @brief Set the Text Color object
         *
         * @param c Color of the line
         * @param bg Color of the background
         */
        void setTextColor(Color c, Color bg);

        /**
         * @brief Set text cursor location
         *
         * @param x X coordinate in pixels
         * @param y Y coordinate in pixels
         */
        void setCursor(int16_t x, int16_t y);

        /**
         * @brief Print one byte/character of data, used to support print()
         *
         * @param c The 8-bit ascii character to write
         * @return size_t
         */
        size_t write(uint8_t c);

        /**
         * @brief Enable (or disable) Code Page 437-compatible charset.
         *
         * @param x true = enable (new behavior), false = disable (old behavior)
         */
        void cp437(bool x);

        /**
         * @brief Set the font to display when print()ing, either custom or default
         *
         * @param f The GFXfont object, if NULL use built in 6x8 font
         */
        void setFont(const GFXfont *f);

    protected:
        // General Data
        uint16_t m_width;
        uint16_t m_height;
        uint8_t m_rotation;                 // Display rotation (0 thru 3)
        uint8_t m_textsize_x;               // Desired magnification in X-axis of text to print()
        uint8_t m_textsize_y;               // Desired magnification in Y-axis of text to print()
        Color m_textcolor;                  // 16-bit background color for print()
        Color m_textbgcolor;                // 16-bit text color for print()
        int16_t m_cursor_x;                 // x location to start print()ing text
        int16_t m_cursor_y;                 // y location to start print()ing text
        bool m_wrap;                        // If set, 'wrap' text at right edge of display
        bool m_cp437 {false};               // If set, use correct CP437 charset (default is off)
        GFXfont *m_gfx_font;                // Pointer to special font
};