#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-status.hpp>
#include <stm-hal/hal-tim.hpp>
#include <gscope/gscope.hpp>
#include <bq25601/bq25601.hpp>
#include <stm-hal/hal-i2c.hpp>

static TaskHandle_t s_task_handler;

static void s_i2c_finish_cb(bool error, void*, uint8_t*, uint16_t)
{
    (void) error;

}

static bool s_i2c_read(uint8_t dir, uint8_t reg, uint8_t*, uint8_t size)
{
    return hal_i2c_read(I2C_TYPE_SYSTEM, dir, reg, size, s_i2c_finish_cb, nullptr);
}

static bool s_i2c_write(uint8_t dir, uint8_t reg, uint8_t* buf, uint8_t size)
{
    return hal_i2c_write(I2C_TYPE_SYSTEM, dir, reg, buf, size, s_i2c_finish_cb, nullptr);
}

static void s_new_notification(BQ25601::Notification notification)
{
    GSDebug("BQ25601 Notification: %d", notification);
}

static constexpr BQ25601::Config s_bq25601_config =
{
    .read_cb = s_i2c_read,
    .write_cb = s_i2c_write,
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

static void s_debug_produce_update()
{
    static uint32_t s_timestamp = 0;
    uint32_t now = hal_timer_32_ms();

    if ((now - s_timestamp) >= 1000)
    {
        s_timestamp = now;
    }
}

static void s_blinky_thread(void*)
{
    s_bq25601.init();

    while(1)
    {
        s_bq25601.update();

        s_debug_produce_update();

        system_status_update();

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

    system_status_init();
    hal_i2c_init(I2C_TYPE_SYSTEM);  // TODO!
}