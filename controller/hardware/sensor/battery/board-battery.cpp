#include <battery/board-battery.hpp>
#include <system/system-status.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC BATTERY CONVERSION API
// ------------------------------------------------------------------------------------

/**
 * Updates the battery conversion with the given voltage.
 * This does a new estimate of battery charge with the given voltage.
 *
 * @param voltage
*/
void BatteryConversion::update(float voltage)
{
    float charge = estimate_charge(voltage);
    m_battery_charge = m_battery_charge * POLE_Z + (1.0f - POLE_Z) * charge;
}

/**
 * Gets the most recently calculate value of the batteries charge.
 *
 * @return charge
*/
float BatteryConversion::get_charge()
{
    return m_battery_charge;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE BATTERY CONVERSION API
// ------------------------------------------------------------------------------------

/**
 * Estimates the current charge of the battery via a lookup table using battery voltage.
 *
 * @param voltage
 * @return charge estimate
*/
float BatteryConversion::estimate_charge(float voltage)
{
    uint8_t idx = 0;
    if (voltage < s_voltage_to_batterty[0])
    {
        return 0.0f;
    }
    else if (voltage >= s_voltage_to_batterty[LOOKUP_TABLE_SIZE - 1])
    {
        return 100.0f;
    }

    for (uint8_t i = 0; i < LOOKUP_TABLE_SIZE; i++)
    {
        if (voltage >= s_voltage_to_batterty[i])
        {
            idx = i;
        }
    }

    // run quick interpolation
    uint8_t percentage_start = idx * s_charge_step_per;
    float voltage_start = s_voltage_to_batterty[idx];
    float voltage_stop = s_voltage_to_batterty[idx + 1];

    float x = (float)s_charge_step_per / (voltage_stop - voltage_start);
    return percentage_start + x * (voltage - voltage_start);
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC BATTERY API
// ------------------------------------------------------------------------------------

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

    m_battery_conversion.update(m_battery_voltage);
    m_battery_charge = m_battery_conversion.get_charge();
}

/**
 * Get whether a battery is connected or not.
 *
 * @return connected
*/
bool Battery::is_connected()
{
    return m_battery_connected;
}

/**
 * Get the battery voltage.
 *
 * @return battery voltage
*/
float Battery::get_voltage()
{
    return m_battery_voltage;
}

/**
 * Get the battery charge percentage.
 *
 * @return battery charge.
*/
float Battery::get_charge()
{
    return m_battery_charge;
}
