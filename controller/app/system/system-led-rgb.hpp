#pragma once

#include <cstdint>
#include <stdbool.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Colour;

#define WHITE   Colour{ .red = 255, .green = 255,   .blue = 255,    };
#define RED     Colour{ .red = 255, .green = 0,     .blue = 0,      };
#define GREEN   Colour{ .red = 0,   .green = 255,   .blue = 0,      };
#define BLUE    Colour{ .red = 0,   .green = 0,     .blue = 255,    };
#define BLACK   Colour{ .red = 0,   .green = 0,     .blue = 0,      };

enum Brightness
{
    BRI_0 = 0,
    BRI_1,  
    BRI_2,
    BRI_3,    
    BRI_4,
    BRI_5,
    BRI_6,
    BRI_7,
    BRI_8,
    BRI_9,
};

class LEDRGB
{
    public:

        LEDRGB(uint16_t io);

        void set_on(void);

        void set_off(void);

        void set_colour(Colour colour);

        Colour get_colour(void);

        void set_brightness(Brightness brightness);

        Brightness get_brightness(void);

        void write_io(bool bit);

        void show();

    private:

        uint16_t m_io;
        
        Colour m_colour;

        Brightness m_brightness;

        bool m_on;
};
