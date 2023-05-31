#include <config/appconfig.h>
#include <gsmenu/gslist.hpp>
#include <gsmenu/gsmenu.hpp>
#include <system/system-freertos.hpp>
#include <system/system-safe-i2c.hpp>
#include <ssd1306/ssd1306.hpp>

#include <stm-hal/hal-gpio.hpp>

static constexpr uint8_t WIDTH = 128;
static constexpr uint8_t HEIGHT = 64;
static uint8_t s_buffer[SSD1306::get_buffer_size(WIDTH, HEIGHT)];
static TaskHandle_t s_task_handler;

static constexpr SSD1306::SSD1306_Config s_config =
{
    .display_width = WIDTH,
    .display_height = HEIGHT,
    .i2c_address = SSD1306_Register::SSD1306_Address::ADDRESS_32BIT_DISPALY,
    .buffer = s_buffer,
    .buffer_size = sizeof(s_buffer),
    .set_reset_reset_line = nullptr,
    .set_reset_cs = nullptr,
    .set_reset_dc = nullptr,
    .i2c_transaction_cb = system_safe_i2c_write,
    .spi_transaction_cb = nullptr,
    .external_vcc = false,  // generate display voltage from 3v3, true othewise
};

static SSD1306 s_ssd1306(s_config);

static constexpr GSListPanel::ContextList s_main_menu_list_context[] =
{
    { "Inputs", nullptr },
    { "Outputs", nullptr },
    { "Battery", nullptr },
    { "Charger", nullptr },
};

static GSListPanel s_main_menu("OpenBotBrain", s_main_menu_list_context,
    ELEMS(s_main_menu_list_context));

static GSMenu s_gsmenu(&s_ssd1306);

static void s_buttons_update()
{
    // TODO: Add debouncer
    if (hal_gpio_read_pin(BUTTON_1_IO) == false)
    {
        s_gsmenu.event(GSPanel::EventType::MOVE_UP);
    }

    if (hal_gpio_read_pin(BUTTON_2_IO) == false)
    {
        s_gsmenu.event(GSPanel::EventType::ENTER);
    }

    if (hal_gpio_read_pin(BUTTON_3_IO) == false)
    {
        s_gsmenu.event(GSPanel::EventType::MOVE_DOWN);
    }
}

static void s_display_thread(void*)
{
    s_ssd1306.init(vTaskDelay);

    // Init main menu interface
    s_gsmenu.init(&s_main_menu);

    while(1)
    {
        s_buttons_update();

        s_gsmenu.update();

        vTaskDelay(10);
    }
}

void task_display_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_DISPLAY];

    s_task_handler = xTaskCreateStatic(s_display_thread, "Display", SIZE_DISPLAY,
        0, PRI_DISPLAY, s_stack, &s_task_buffer);
}