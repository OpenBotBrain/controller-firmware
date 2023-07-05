#pragma once

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

void system_neoled_init(void);

void system_neoled_update(void);

void system_neoled_on(void);

void system_neoled_off(void);

void system_neoled_set_brightness(Neoled_Brightness brightness);

Neoled_Brightness system_neoled_get_brightness(void);

void system_neoled_set_rgb(uint8_t r, uint8_t g, uint8_t b);

void system_neoled_set_colour(Neoled_Colour colour);

Neoled_Colour system_neoled_get_colour(void);
