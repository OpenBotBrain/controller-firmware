#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <hardware/hardware-manager.hpp>
#include <map>
#include <cstdlib>
#include <ctime>

static const TickType_t S_TICK_DELAY = 5;
static const TickType_t S_LED_SHOW_TIME = 1000;
static const TickType_t S_BUTTON_BOUNCE_TIME = 100;
static TaskHandle_t s_task_handler;

static HardwareManager s_hardware_manager;
static NeoLED s_neoled;
static LED s_led;
static Buttons s_buttons;

static const uint8_t S_LED_COUNT = 3;
static LED_Type s_led_types[S_LED_COUNT] =
{
    LED_Type::TOP,
    LED_Type::MIDDLE,
    LED_Type::BOTTOM
};

static const uint8_t S_BUTTON_COUNT = 3;
static Button_Type s_button_types[S_BUTTON_COUNT] =
{
    Button_Type::TOP,
    Button_Type::MIDDLE,
    Button_Type::BOTTOM
};

static NeoLED_Colour s_colour;
static bool s_user_correct = true;
static uint8_t s_random_number = 0;
static uint32_t s_user_wins = 0;

static bool s_get_user_press(LED_Type led_type)
{
    bool button_pressed = false;

    for ( ;; )
    {
        s_buttons.update();

        for (uint8_t i = 0; i < S_BUTTON_COUNT; i++)
        {
            if (s_buttons.is_pressed(s_button_types[i]))
            {
                if (led_type == s_led_types[i])
                {
                    return true;
                }
                button_pressed = true;
                vTaskDelay(S_BUTTON_BOUNCE_TIME);
            }
        }

        if (button_pressed)
        {
            return false;
        }

        vTaskDelay(S_TICK_DELAY);
    }

    return false;
}

/**
 * Memory Game task thread.
*/
static void s_memory_game_thread(void*)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    s_hardware_manager.init();
    s_neoled = s_hardware_manager.get_neoled();
    s_led = s_hardware_manager.get_led();
    s_buttons = s_hardware_manager.get_buttons();

    s_neoled.init();
    s_neoled.set_state(true);
    s_led.init();
    s_buttons.init();

    for ( ;; )
    {
        // Reset loop
        s_neoled.set_colour(NEO_BLACK);
        s_neoled.update();
        s_led.reset();

        s_user_correct = true;
        uint32_t round_count = 0;
        while (round_count < s_user_wins + 1 && s_user_correct)
        {
            // Generate random number between 1 and 3
            s_random_number = std::rand() % 3;
            // Show the pattern
            s_led.set_led_state(s_led_types[s_random_number], true);
            s_led.update();
            vTaskDelay(S_LED_SHOW_TIME);
            s_led.set_led_state(s_led_types[s_random_number], false);
            s_led.update();

            s_user_correct = s_get_user_press(s_led_types[s_random_number]);
            round_count++;
        }

        // Shine RED or GREEN on NeoLED based on user win or lose.
        if (s_user_correct)
        {
            s_colour = NEO_GREEN;
            s_user_wins++;
        }
        else
        {
            s_colour = NEO_RED;
        }

        s_neoled.set_brightness(NEO_BRI_1);
        s_neoled.set_colour(s_colour);
        s_neoled.update();
        vTaskDelay(S_LED_SHOW_TIME);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_memory_game_init()
{
    // Create task for Memory Game App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_MEM_GAME];

    s_task_handler = xTaskCreateStatic(
        s_memory_game_thread,
        "Memory Game",
        SIZE_MEM_GAME,
        0,
        PRI_MEM_GAME,
        s_stack,
        &s_task_buffer
    );
}
