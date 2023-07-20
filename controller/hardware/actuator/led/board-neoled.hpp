#pragma once

#include <actuator/hardware-actuator.hpp>
#include <cstdint>

static constexpr uint8_t COLOUR_SIZE        = 8;  // Size of one colour.
static constexpr uint8_t COLOUR_BUFFER_SIZE = 25; // 3 * COLOUR_SIZE + 1.

/**
 * MAX is NEO_BRI_10, MIN is NEO_BRI_1.
 *
 * MIN brightness will not turn off LED.
*/
enum Neoled_Brightness
{
    NEO_BRI_UNDEFINED = 0,
    NEO_BRI_10,
    NEO_BRI_9,
    NEO_BRI_8,
    NEO_BRI_7,
    NEO_BRI_6,
    NEO_BRI_5,
    NEO_BRI_4,
    NEO_BRI_3,
    NEO_BRI_2,
    NEO_BRI_1,
};

/**
 * Store Colour values for RGB LED.
 *
 * @param red Red value (0 - 255).
 * @param green Green value (0 - 255).
 * @param blue Blue value (0 - 255).
*/
struct Neoled_Colour
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

static constexpr Neoled_Colour NEO_WHITE    { .red = 255,  .green = 255,   .blue = 255  };
static constexpr Neoled_Colour NEO_RED      { .red = 255,  .green = 0,     .blue = 0    };
static constexpr Neoled_Colour NEO_GREEN    { .red = 0,    .green = 255,   .blue = 0    };
static constexpr Neoled_Colour NEO_BLUE     { .red = 0,    .green = 0,     .blue = 255  };
static constexpr Neoled_Colour NEO_YELLOW   { .red = 255,  .green = 255,   .blue = 0    };
static constexpr Neoled_Colour NEO_PURPLE   { .red = 255,  .green = 0,     .blue = 255  };
static constexpr Neoled_Colour NEO_TEAL     { .red = 0,    .green = 255,   .blue = 255  };
static constexpr Neoled_Colour NEO_BLACK    { .red = 0,    .green = 0,     .blue = 0    };

class Neoled : public Actuator
{
    public:

        Neoled() {};

        virtual void init() override;

        virtual void update() override;

        void set_enable(bool enable);

        void set_brightness(Neoled_Brightness brightness);

        Neoled_Brightness get_brightness();

        void set_rgb(uint8_t r, uint8_t g, uint8_t b);

        void set_colour(Neoled_Colour colour);

        Neoled_Colour get_colour();

    private:

        void load_rgb();

        bool colour_changed();

    private:

        bool m_rgb_on {false};

        Neoled_Colour m_rgb_colour {NEO_WHITE};

        Neoled_Colour m_last_colour {NEO_BLACK};

        Neoled_Brightness m_rgb_brightness {NEO_BRI_1};

        uint8_t m_rgb_timer_on_off_periods[2] = {0};

        uint8_t m_rgb_timer_data[COLOUR_BUFFER_SIZE] = {0};

        uint32_t m_timestamp;
};
