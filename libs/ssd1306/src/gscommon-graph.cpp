#include <ssd1306/gscommon-graph.hpp>

uint8_t GSCommonGraph::getRotation()
{
    return m_rotation;
}

uint8_t GSCommonGraph::width()
{
    return m_width;
}

uint8_t GSCommonGraph::height()
{
    return m_height;
}

void GSCommonGraph::swap(int16_t& a, int16_t& b)
{
    int16_t temp;
    temp = b;
    b = a;
    a = temp;
}

void GSCommonGraph::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap,
    int16_t w, int16_t h, Color color)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t b = 0;

    for (int16_t j = 0; j < h; j++, y++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            if (i & 7)
            {
                b <<= 1;
            }
            else
            {
                b = bitmap[j * byteWidth + i / 8];
            }
            if (b & 0x80)
            {
                drawPixel(x + i, y, color);
            }
        }
    }
}

void GSCommonGraph::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color)
{
    int16_t steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            drawPixel(y0, x0, color);
        }
        else
        {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void GSCommonGraph::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color)
{
    // Update in subclasses if desired!
    if (x0 == x1)
    {
        if (y0 > y1)
        {
            swap(y0, y1);
        }
        drawFastVLine(x0, y0, y1 - y0 + 1, color);
    }
    else if (y0 == y1)
    {
        if (x0 > x1)
        {
            swap(x0, x1);
        }
        drawFastHLine(x0, y0, x1 - x0 + 1, color);
    }
    else
    {
        writeLine(x0, y0, x1, y1, color);
    }
}

void GSCommonGraph::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color)
{
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

void GSCommonGraph::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color)
{
    for (int i = x; i < x + w; i++)
    {
        drawFastVLine(i, y, h, color);
    }
}

void GSCommonGraph::fillScreen(Color color)
{
    fillRect(0, 0, m_width, m_height, color);
}

void GSCommonGraph::drawCircle(int16_t x0, int16_t y0, int16_t r, Color color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void GSCommonGraph::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, Color color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x4)
        {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2)
        {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8)
        {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1)
        {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void GSCommonGraph::fillCircle(int16_t x0, int16_t y0, int16_t r, Color color)
{
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void GSCommonGraph::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t corners, int16_t delta, Color color)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (x < (y + 1))
        {
            if (corners & 1)
            {
                drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
            }
            if (corners & 2)
            {
                drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
            }
        }
        if (y != py)
        {
            if (corners & 1)
            {
                drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
            }
            if (corners & 2)
            {
                drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
            }
            py = y;
        }
        px = x;
    }
}

void GSCommonGraph::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
    {
        r = max_radius;
    }
    // smarter version
    drawFastHLine(x + r, y, w - 2 * r, color);         // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c)
{
    return gfxFont->glyph + c;
}

inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont)
{
    return gfxFont->bitmap;
}

void GSCommonGraph::drawChar(int16_t x, int16_t y, unsigned char c, Color color,
    Color bg, uint8_t size_x, uint8_t size_y)
{
    if (!m_gfx_font)
    {
        // 'Classic' built-in font
        if ((x >= m_width) ||    // Clip right
            (y >= m_height) ||   // Clip bottom
            ((x + 6 * size_x - 1) < 0) ||       // Clip left
            ((y + 8 * size_y - 1) < 0))         // Clip top
        {
            return;
        }

        if (!m_cp437 && (c >= 176))
        {
            c++; // Handle 'classic' charset behavior
        }

        for (int8_t i = 0; i < 5; i++)
        {
            // Char bitmap = 5 columns
            const unsigned char* font = get_glcd_font();
            uint8_t line = font[c * 5 + i];
            for (int8_t j = 0; j < 8; j++, line >>= 1)
            {
                if (line & 1)
                {
                    if (size_x == 1 && size_y == 1)
                    {
                        drawPixel(x + i, y + j, color);
                    }
                    else
                    {
                        fillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
                    }
                }
                else if (bg != color)
                {
                    if (size_x == 1 && size_y == 1)
                    {
                        drawPixel(x + i, y + j, bg);
                    }
                    else
                    {
                        fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
                    }
                }
            }
        }
        if (bg != color)
        {
            // If opaque, draw vertical line for last column
            if (size_x == 1 && size_y == 1)
            {
                drawFastVLine(x + 5, y, 8, bg);
            }
            else
            {
                fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
            }
        }
    }
    else
    {
        // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!
        c -= (uint8_t)m_gfx_font->first;
        GFXglyph *glyph = pgm_read_glyph_ptr(m_gfx_font, c);
        uint8_t *bitmap = pgm_read_bitmap_ptr(m_gfx_font);

        uint16_t bo = glyph->bitmapOffset;
        uint8_t w = glyph->width;
        uint8_t h = glyph->height;
        int8_t xo = glyph->xOffset;
        int8_t yo = glyph->yOffset;
        uint8_t xx, yy, bits = 0, bit = 0;
        int16_t xo16 = 0, yo16 = 0;

        if (size_x > 1 || size_y > 1)
        {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here
        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        for (yy = 0; yy < h; yy++)
        {
            for (xx = 0; xx < w; xx++)
            {
                if (!(bit++ & 7))
                {
                    bits = bitmap[bo++];
                }
                if (bits & 0x80)
                {
                    if (size_x == 1 && size_y == 1)
                    {
                        drawPixel(x + xo + xx, y + yo + yy, color);
                    }
                    else
                    {
                        fillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                        size_x, size_y, color);
                    }
                }
                bits <<= 1;
            }
        }
    } // End classic vs custom font
}

void GSCommonGraph::setTextSize(uint8_t s)
{
    setTextSize(s, s);
}

void GSCommonGraph::setTextSize(uint8_t s_x, uint8_t s_y)
{
    m_textsize_x = (s_x > 0) ? s_x : 1;
    m_textsize_y = (s_y > 0) ? s_y : 1;
}

void GSCommonGraph::setTextColor(Color c)
{
    m_textcolor = m_textbgcolor = c;
}

void GSCommonGraph::setTextColor(Color c, Color bg)
{
    m_textcolor = c;
    m_textbgcolor = bg;
}

void GSCommonGraph::setCursor(int16_t x, int16_t y)
{
    m_cursor_x = x;
    m_cursor_y = y;
}

size_t GSCommonGraph::write(uint8_t c)
{
    if (!m_gfx_font)
    {
        // 'Classic' built-in font
        if (c == '\n')
        {
            // Newline?
            m_cursor_x = 0;                     // Reset x to zero,
            m_cursor_y += m_textsize_y * 8;     // advance y one line
        }
        else if (c != '\r')
        {
            // Ignore carriage returns
            if (m_wrap && ((m_cursor_x + m_textsize_x * 6) > m_width))
            {
                // Off right?
                m_cursor_x = 0;                                       // Reset x to zero,
                m_cursor_y += m_textsize_y * 8; // advance y one line
            }
            drawChar(m_cursor_x, m_cursor_y, c, m_textcolor, m_textbgcolor, m_textsize_x, m_textsize_y);
            m_cursor_x += m_textsize_x * 6; // Advance x one char
        }

    }
    else
    {
        // Custom font

        if (c == '\n')
        {
            m_cursor_x = 0;
            m_cursor_y += (int16_t)m_textsize_y * (uint8_t)m_gfx_font->yAdvance;
        }
        else if (c != '\r')
        {
            uint8_t first = m_gfx_font->first;
            if ((c >= first) && (c <= (uint8_t)m_gfx_font->last))
            {
                GFXglyph *glyph = pgm_read_glyph_ptr(m_gfx_font, c - first);
                uint8_t w = glyph->width;
                uint8_t h = glyph->height;
                if ((w > 0) && (h > 0))
                {
                    // Is there an associated bitmap?
                    int16_t xo = (int8_t)glyph->xOffset; // sic
                    if (m_wrap && ((m_cursor_x + m_textsize_x * (xo + w)) > m_width))
                    {
                        m_cursor_x = 0;
                        m_cursor_y += (int16_t)m_textsize_y * (uint8_t)m_gfx_font->yAdvance;
                    }
                    drawChar(m_cursor_x, m_cursor_y, c, m_textcolor, m_textbgcolor, m_textsize_x, m_textsize_y);
                }
                m_cursor_x += (uint8_t)glyph->xAdvance * (int16_t)m_textsize_x;
            }
        }
    }
    return 1;
}

void GSCommonGraph::cp437(bool x)
{
    m_cp437 = x;
}

void GSCommonGraph::setFont(const GFXfont *f)
{
    if (f)
    {
        // Font struct pointer passed in?
        if (!m_gfx_font)
        {
            // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            m_cursor_y += 6;
        }
    }
    else if (m_gfx_font)
    {
        // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        m_cursor_y -= 6;
    }
    m_gfx_font = (GFXfont *)f;
}
