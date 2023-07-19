#include <hardware/hardware-manager.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Creates the HardwareManager object, inits its members.
*/
HardwareManager::HardwareManager(void)
{
    Neoled neoled;
    m_actuators[NEOLED] = &neoled;

    Led led;
    m_actuators[LED] = &led;

    IMU imu;
    m_sensors[GYRO] = &imu;

    m_hardware_config = Hardware_Config
    {
        .neoled_update_interval = 35,
        .led_update_interval = 250,
        .imu_update_interval = 5,
    };
}

/**
 * Init everything within the hardware manager.
*/
void HardwareManager::init()
{
    if (m_neoled != nullptr)
    {
        m_neoled->init();
    }

    if (m_led != nullptr)
    {
        m_led->init();
    }

    if (m_imu != nullptr)
    {
        m_imu->init();
    }

    if (m_battery != nullptr)
    {
        m_battery->init();
    }

    // If no battery connected do not init any ports.
    if (!m_battery->battery_connected())
    {
        return;
    }
}

/**
 * Update everything within the hardware manager.
*/
void HardwareManager::update()
{
    if (m_neoled != nullptr)
    {
        m_neoled->update();
    }

    if (m_led != nullptr)
    {
        m_led->update();
    }

    if (m_imu != nullptr)
    {
        m_imu->update();
    }

    if (m_battery != nullptr)
    {
        m_battery->update();
    }

    // If no battery connected do not update any ports.
    if (!m_battery->battery_connected())
    {
        return;
    }
}

/**
 * Gets the config of the hardware manager.
 *
 * @return Hardware_Config
*/
Hardware_Config HardwareManager::get_hardware_config()
{
    return m_hardware_config;
}

/**
 * Get pointer of actuator a.
 *
 * @return LegoMotor*
*/
Actuator *HardwareManager::get_actuator(Acuator_Type actuator)
{
    return m_actuators[actuator];
}


Sensor *HardwareManager::get_sensor(Sensor_Type sensor)
{
    return m_sensors[sensor];
}


// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
