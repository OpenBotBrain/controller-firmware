#include <battery/board-battery.hpp>
#include <system/system-status.hpp>

/**
 * Init the battery.
*/
void Battery::init()
{
    system_status_init();

    m_battery_init = true;
}

/**
 * Update the battery.
*/
void Battery::update()
{
    if (!m_battery_init)
    {
        return;
    }

    system_status_update();
    m_battery_voltage = system_get_vbat_voltage();

    m_battery_connected = m_battery_voltage >= BATTERY_VOLTAGE;
}

/**
 * Get whether a battery is connected or not.
 *
 * @return connected
*/
bool Battery::connected()
{
    return m_battery_connected;
}

/**
 * Get the battery voltage.
 *
 * @return battery voltage
*/
float Battery::voltage()
{
    return m_battery_voltage;
}

/**
 * Get the battery charge percentage.
 *
 * @return battery charge.
*/
float Battery::charge()
{
    m_battery_conversion.update_new_value(m_battery_voltage);
    return m_battery_conversion.get_charge();
}
