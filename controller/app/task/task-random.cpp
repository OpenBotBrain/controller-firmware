#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-rng.hpp>
#include <hardware/hardware-manager.hpp>
#include <gscope/gscope.hpp>
#include <random>

static const TickType_t S_TICK_DELAY = 5;
static const TickType_t S_LED_SHOW_TIME = 500;
static TaskHandle_t s_task_handler;

static HardwareManager s_hardware_manager;
static LED s_led;

static const uint8_t S_LED_COUNT = 3;
static LED_Type s_led_types[S_LED_COUNT] =
{
    LED_Type::TOP,
    LED_Type::MIDDLE,
    LED_Type::BOTTOM
};

static void s_random_thread(void*)
{
    uint32_t seed = hal_rng_get_once();
    GSDebug("seed: %d", seed);
    std::srand(seed);

    s_hardware_manager.init();
    s_led = s_hardware_manager.get_led();

    s_led.init();

    for ( ;; )
    {
        uint32_t random_number = std::rand() % 3;
        GSDebug("random number %d", random_number);

        s_led.reset();
        s_led.set_led_state(s_led_types[random_number], true);
        s_led.update();

        vTaskDelay(S_LED_SHOW_TIME);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

void task_random_init()
{
    // Create task for Random task
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_RANDOM];

    s_task_handler = xTaskCreateStatic(
        s_random_thread,
        "Random",
        SIZE_RANDOM,
        0,
        PRI_RANDOM,
        s_stack,
        &s_task_buffer
    );
}
