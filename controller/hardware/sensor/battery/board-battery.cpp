#include <battery/board-battery.hpp>

/**
 * Init the battery.
*/
void Battery::init()
{

}

/**
 * Update the battery.
*/
void Battery::update()
{

}

/**
 * Get whether a battery is connected or not.
 *
 * @return bool
*/
bool Battery::battery_connected()
{
    return m_battery_connected;
}

/**
 * Get the battery voltage.
 *
 * @return float
*/
float Battery::battery_voltage()
{
    return m_battery_voltage;
}
