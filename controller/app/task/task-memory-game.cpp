#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <hardware/hardware-manager.hpp>
#include <map>

static const TickType_t S_TICK_DELAY = 5;
static const TickType_t S_LED_SHOW_TIME = 1000;
static const TickType_t S_BUTTON_BOUNCE_TIME = 200;
static TaskHandle_t s_task_handler;

static HardwareManager s_hardware_manager;
static NeoLED s_neoled;
static LED s_led;
static Buttons s_buttons;

static const uint8_t S_MEMORY_PATTERN_LENGTH = 3;
static LED_Type s_led_order[S_MEMORY_PATTERN_LENGTH] =
{
    LED_Type::TOP,
    LED_Type::MIDDLE,
    LED_Type::BOTTOM
};
static NeoLED_Colour s_colour;

static std::map<LED_Type, Button_Type> s_led_button_map;
static bool s_user_correct = true;

static bool s_get_user_guess(LED_Type led_type)
{
    bool button_pressed = false;

    for ( ;; )
    {
        s_buttons.update();

        if (s_buttons.is_pressed(Button_Type::TOP))
        {
            if (led_type == LED_Type::TOP)
            {
                return true;
            }
            button_pressed = true;
            s_neoled.set_colour(NEO_YELLOW);
            s_neoled.update();
            vTaskDelay(S_BUTTON_BOUNCE_TIME);
        }
        else if (s_buttons.is_pressed(Button_Type::MIDDLE))
        {
            if (led_type == LED_Type::MIDDLE)
            {
                return true;
            }
            button_pressed = true;
            s_neoled.set_colour(NEO_TEAL);
            s_neoled.update();
            vTaskDelay(S_BUTTON_BOUNCE_TIME);
        }
        else if (s_buttons.is_pressed(Button_Type::BOTTOM))
        {
            if (led_type == LED_Type::BOTTOM)
            {
                return true;
            }
            button_pressed = true;
            s_neoled.set_colour(NEO_PURPLE);
            s_neoled.update();
            vTaskDelay(S_BUTTON_BOUNCE_TIME);
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
    s_led_button_map[LED_Type::TOP] = Button_Type::TOP;
    s_led_button_map[LED_Type::MIDDLE] = Button_Type::MIDDLE;
    s_led_button_map[LED_Type::BOTTOM] = Button_Type::BOTTOM;

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
        s_neoled.set_colour(NEO_BLACK);
        s_neoled.update();
        s_led.reset();

        // Show the pattern
        for (uint8_t i = 0; i < S_MEMORY_PATTERN_LENGTH; i++)
        {
            s_led.set_led_state(s_led_order[i], true);
            s_led.update();
            vTaskDelay(S_LED_SHOW_TIME);
            s_led.set_led_state(s_led_order[i], false);
            s_led.update();
        }

        // User recreates the pattern with the buttons.
        for (uint8_t i = 0; i < S_MEMORY_PATTERN_LENGTH; i++)
        {
            s_neoled.set_colour(NEO_BLACK);
            s_neoled.update();
            bool guess_correct = s_get_user_guess(s_led_order[i]);

            if (!guess_correct)
            {
                s_user_correct = false;
            }
        }

        // Shine RED or GREEN on NeoLED based on user win or lose.
        s_colour = (s_user_correct) ? NEO_GREEN : NEO_RED;
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
