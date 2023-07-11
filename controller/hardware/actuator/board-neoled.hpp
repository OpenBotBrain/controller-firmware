#pragma once

#include <actuator/hardware-actuator.hpp>
#include <cstdint>

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

#define NEO_WHITE   Neoled_Colour{ .red = 255,  .green = 255,   .blue = 255 }
#define NEO_RED     Neoled_Colour{ .red = 255,  .green = 0,     .blue = 0   }
#define NEO_GREEN   Neoled_Colour{ .red = 0,    .green = 255,   .blue = 0   }
#define NEO_BLUE    Neoled_Colour{ .red = 0,    .green = 0,     .blue = 255 }
#define NEO_YELLOW  Neoled_Colour{ .red = 255,  .green = 255,   .blue = 0   }
#define NEO_PURPLE  Neoled_Colour{ .red = 255,  .green = 0,     .blue = 255 }
#define NEO_TEAL    Neoled_Colour{ .red = 0,    .green = 255,   .blue = 255 }

class Neoled : public Actuator
{
    public:

        Neoled(void);

        void init(void);

        void update(void);

        void on(void);

        void off(void);

        void set_brightness(Neoled_Brightness brightness);

        Neoled_Brightness get_brightness(void);

        void set_rgb(uint8_t r, uint8_t g, uint8_t b);

        void set_colour(Neoled_Colour colour);

        Neoled_Colour get_colour(void);

    private:
        
        bool m_rgb_on;

        Neoled_Colour m_rgb_colour;

        Neoled_Brightness m_rgb_brightness;  
};
