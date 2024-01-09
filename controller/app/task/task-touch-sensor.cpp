#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-tim.hpp>
#include <touch/ev3-touch-sensor.hpp>
#include <touch/nxt-touch-sensor.hpp>
#include <module/module-input.hpp>
#include <hardware/hardware-manager.hpp>

static TaskHandle_t s_task_handler;

static HardwareManager s_manager;
static Hardware_Config s_hardware_config;
static LED s_led;
static NXTTouchSensor s_nxt_touch_sensor = NXTTouchSensor(Lego_Sensor_Port::SENSOR_1);
static EV3TouchSensor s_ev3_touch_sensor = EV3TouchSensor(Lego_Sensor_Port::SENSOR_4);

/**
 * Touch sensor task.
 *
 * Will turn on one of the LEDs if the touch sensor is being pressed.
*/
static void s_touch_sensor_thread(void *)
{
    s_nxt_touch_sensor.init();
    s_ev3_touch_sensor.init();

    s_manager.init();

    s_hardware_config = s_manager.get_hardware_config();
    s_led = s_manager.get_led();

    for ( ;; )
    {
        uint32_t now = hal_timer_32_ms();
        static uint32_t s_led_timestamp;

        if ((now - s_led_timestamp) >= s_hardware_config.led_update_interval)
        {
            s_led_timestamp = now;

            s_led.set_led_state(LED_Type::TOP, s_nxt_touch_sensor.fetch_sample() >= 1.0f);
            s_led.set_led_state(LED_Type::MIDDLE, s_ev3_touch_sensor.fetch_sample() >= 1.0f);
            s_led.set_led_state(LED_Type::BOTTOM, !s_led.get_led_state(LED_Type::BOTTOM));

            s_led.update();
        }

        static uint32_t s_sensor_timestamp;

        if ((now - s_sensor_timestamp) >= s_hardware_config.sensor_update_interval)
        {
            s_sensor_timestamp = now;
            s_nxt_touch_sensor.update();
            s_ev3_touch_sensor.update();
        }
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

void task_touch_sensor_init()
{
    // Create touch sensor task
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_HARDWARE_MAN];

    s_task_handler = xTaskCreateStatic(
        s_touch_sensor_thread,
        "Hardware",
        SIZE_HARDWARE_MAN,
        0,
        PRI_HARDWARE_MAN,
        s_stack,
        &s_task_buffer
    );
}
