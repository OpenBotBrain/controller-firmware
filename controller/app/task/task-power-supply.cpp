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
            noty = "S: Init success";
            break;
        case BQ25601::Notification::HEALTH_GOOD:
            noty = "S: Health good";
            break;
        case BQ25601::Notification::HEALTH_UNSPEC_FAILURE:
            noty = "S: Health failure";
            break;
        case BQ25601::Notification::HEALTH_OVERVOLTAGE:
            noty = "S: Health overvoltage";
            break;
        case BQ25601::Notification::HEALTH_OVERHEAT:
            noty = "S: Health overheat";
            break;
        case BQ25601::Notification::HEALTH_SAFETY_TIMER:
            noty = "S: Health safety timer";
            break;
        case BQ25601::Notification::HEALTH_UNKNOWN:
            noty = "S: Health unknown";
            break;
        case BQ25601::Notification::ERROR_HARDWARE_INIT_FAIL:
            noty = "E: Init FAIL";
            break;
    }

    GSDebug("[BQ25601] Notification: %d - %s", notification, noty);
}

static constexpr BQ25601::Config s_bq25601_config =
{
    .read_cb = system_safe_i2c_read,
    .write_cb = system_safe_i2c_write,
    .delay_ms_cb = vTaskDelay,
    .get_timestamp_ms32_cb = hal_timer_32_ms,
    .notification_cb = s_new_notification
};

static constexpr BQ25601::DriverConfig s_bq25601_driver_default_config =
{
    .temp_protection = BQ25601::TempProtection::TEMP_PROTECTION_90C,
    .charger_setpoint_ma = 2000,     // max 3 amps
    .charger_voltage_mv = 4200,
    .input_current_limit_ma = 1500,  // max 3.2 amps
    .boost_setpoint = BQ25601::BoostSetpoint::VOLTAGE_5_00V,
    .boost_current_setpoint = BQ25601::BoostCurrentLimit::CURRENT_LIMIT_500_MA,
};

static BQ25601 s_bq25601(s_bq25601_config, s_bq25601_driver_default_config);

static void s_charger_irq_callback(void*)
{
    s_bq25601.irq_handler();
}

static void s_charger_debug_produce()
{
    static uint32_t s_last_timestamp = 0;

    const BQ25601::Data& charger_data = s_bq25601.get_status();

    // update data only when we have new information
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

        // Add some debuging on the console when charger is connected or not
        static bool s_last_power_good = false;
        bool power_good = charger_data.system_status_register & BQ25601_REG_SS_PG_STAT_MASK;
        if (s_last_power_good != power_good)
        {
            s_last_power_good = power_good;
            GSDebug("[CHARGER] Input power is %s", power_good ? "Connected" : "Disconnected");
        }

        // indicate some charging states
        static uint8_t s_last_charging_status = 0;
        uint8_t chargin_status = (charger_data.system_status_register & BQ25601_REG_SS_CHRG_STAT_MASK) >> BQ25601_REG_SS_CHRG_STAT_SHIFT;
        if (s_last_charging_status != chargin_status)
        {
            s_last_charging_status = chargin_status;
            const char* stat;
            switch (chargin_status)
            {
                default: stat = "STOPPED"; break;
                case 0x01: stat = "Pre-Charge - Battery Low"; break;
                case 0x02: stat = "FAST"; break;
                case 0x03: stat = "DONE"; break;
            }
            GSDebug("[CHARGER] Charging %s", stat);
        }
    }
}

static void s_check_shutdown_update()
{
    static bool s_pushed = false;
    static bool s_done = false;
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
        else if ((now - s_timestamp) >= 1000 && !s_done)
        {
            s_done = true;
            GSDebug("Shutting the system down");
            s_bq25601.set_system_shutdown();
        }
    }
    else
    {
        s_pushed = false;
        s_done = false;
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

    system_status_init();

    hal_exti_init(BM_NIRQ_IO, TriggerType::FALLING, s_charger_irq_callback, nullptr);
}

// Debug functions
static void s_shutdown_vbat_charger()
{
    s_bq25601.set_system_shutdown();
}
GScopeButton("Shutdown", s_shutdown_vbat_charger)
GScopeCommand("shutdown", s_shutdown_vbat_charger)

static void s_set_max_input_current(int current_ma)
{
    s_bq25601.set_max_input_current(current_ma);
}
GScopeCommand("charger_set_input_current", s_set_max_input_current)

static void s_enable_boost_power(bool enable)
{
    s_bq25601.set_boost_power_supply_enabled(enable);
}
GScopeCommand("charger_boost_enable", s_enable_boost_power)
