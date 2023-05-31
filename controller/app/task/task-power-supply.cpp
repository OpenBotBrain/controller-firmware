#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-status.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-exti.hpp>
#include <gscope/gscope.hpp>
#include <bq25601/bq25601.hpp>
#include <system/system-safe-i2c.hpp>

static TaskHandle_t s_task_handler;
GScopeChannel(s_charger_status, "charger_status", uint8_t, 5)

static void s_new_notification(BQ25601::Notification notification)
{
    const char* noty = "?";

    switch(notification)
    {
        default:
            return;
        case BQ25601::Notification::STATUS_INIT_SUCCESS:
            noty = "Init success";
            break;
        case BQ25601::Notification::STATUS_CHARGER_SYSTEM_STATUS_CHANGED:
            noty = "status changed";
            break;
        case BQ25601::Notification::STATUS_CHARGER_FAULT_CHANGED:
            noty = "fault changed";
            break;
        case BQ25601::Notification::HEALTH_GOOD:
            noty = "Health good";
            break;
        case BQ25601::Notification::HEALTH_UNSPEC_FAILURE:
            noty = "Health failure";
            break;
        case BQ25601::Notification::HEALTH_OVERVOLTAGE:
            noty = "Health overvoltage";
            break;
        case BQ25601::Notification::HEALTH_OVERHEAT:
            noty = "Health overheat";
            break;
        case BQ25601::Notification::HEALTH_SAFETY_TIMER:
            noty = "Health safety timer";
            break;
        case BQ25601::Notification::HEALTH_UNKNOWN:
            noty = "Health unknown";
            break;
        case BQ25601::Notification::ERROR_READING_I2C:
            noty = "E: reading i2c";
            break;
        case BQ25601::Notification::ERROR_WRITING_I2C:
            noty = "E: writing i2c";
            break;
        case BQ25601::Notification::ERROR_READING_SS_REGISTER:
            noty = "E: SS register";
            break;
        case BQ25601::Notification::ERROR_READING_F_REGISTER:
            noty = "E: F register";
            break;
        case BQ25601::Notification::ERROR_CAN_NOT_ACCESS_ISC_REGISTER:
            noty = "E: I2C Register";
            break;
        case BQ25601::Notification::ERROR_VALIDATING_MODEL_NUMBER:
            noty = "E: invalid model number";
            break;
        case BQ25601::Notification::ERROR_RESETING_REGISTERS:
            noty = "E: reseting register";
            break;
        case BQ25601::Notification::ERROR_HARDWARE_INIT_FAIL:
            noty = "E: Init FAIL";
            break;
    }

    GSDebug("BQ25601 Notification: %d - %s", notification, noty);
}

static constexpr BQ25601::Config s_bq25601_config =
{
    .read_cb = system_safe_i2c_read,
    .write_cb = system_safe_i2c_write,
    .delay_ms_cb = vTaskDelay,
    .get_timestamp_ms32_cb = hal_timer_32_ms,
    .notification_cb = s_new_notification
};

static constexpr BQ25601::DriverConfig s_bq25601_driver_config =
{
    .temp_protection = BQ25601::TempProtection::TEMP_PROTECTION_90C,
    .charger_setpoint_ua = 1000000,
    .charger_voltage_uv = 4200000,
    .input_current_limit_ua = 1500000,
    .boost_setpoint = BQ25601::BoostSetpoint::VOLTAGE_5_00V,
};

static BQ25601 s_bq25601(s_bq25601_config, s_bq25601_driver_config);

static void s_shutdown()
{
    s_bq25601.set_system_shutdown();
}
GScopeButton("Shutdown", s_shutdown)

static void s_charger_irq_callback(void*)
{
    s_bq25601.irq_handler();
}

static void s_charger_debug_produce()
{
    static uint32_t s_last_timestamp = 0;

    const BQ25601::Data& charger_data = s_bq25601.get_status();

    if (s_last_timestamp != charger_data.timestamp)
    {
        s_last_timestamp = charger_data.timestamp;

        uint8_t print_data[5];
        print_data[0] = charger_data.system_status_register;
        print_data[1] = charger_data.fault_register;
        print_data[2] = static_cast<uint8_t>(charger_data.temp_protection);
        print_data[3] = static_cast<uint8_t>(charger_data.charger_type);
        print_data[4] = static_cast<uint8_t>(charger_data.charger_health);
        s_charger_status.produce(print_data);
    }
}

static void s_check_shutdown_update()
{
    static bool s_pushed = false;
    static uint32_t s_timestamp;

    bool pushed = hal_gpio_read_pin(SHUTDOWN_IO);

    if (pushed)
    {
        uint32_t now = hal_timer_32_ms();
        if (!s_pushed)
        {
            s_pushed = true;
            s_timestamp = now;
        }
        else if ((now - s_timestamp) >= 1000)
        {
            GSDebug("Entering to sleep");
            s_shutdown();
        }
    }
    else
    {
        s_pushed = false;
    }

}

static void s_blinky_thread(void*)
{
    s_bq25601.init();

    while(1)
    {
        s_bq25601.update();

        s_charger_debug_produce();

        system_status_update();

        s_check_shutdown_update();

        vTaskDelay(25);
    }
}


void task_power_supply_init()
{
    // Create task for Blinky App
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_POWER_sUPPLY];

    s_task_handler = xTaskCreateStatic(s_blinky_thread, "Power Supply", SIZE_POWER_sUPPLY,
        0, PRI_POWER_SUPPLY, s_stack, &s_task_buffer);

    system_safe_i2c_init();

    hal_exti_init(BM_NIRQ_IO, TriggerType::FALLING, s_charger_irq_callback, nullptr);
}