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
    m_neoled = &neoled;

    Led led;
    m_led = &led;

    IMU imu;
    m_imu = &imu;

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
LegoMotor *HardwareManager::get_actuator_a(void)
{
    return nullptr;
}

/**
 * Get pointer of actuator b.
 *
 * @return LegoMotor*
*/
LegoMotor *HardwareManager::get_actuator_b(void)
{
    return nullptr;
}

/**
 * Get pointer of actuator c.
 *
 * @return LegoMotor*
*/
LegoMotor *HardwareManager::get_actuator_c(void)
{
    return nullptr;
}

/**
 * Get pointer of actuator d.
 *
 * @return LegoMotor*
*/
LegoMotor *HardwareManager::get_actuator_d(void)
{
    return nullptr;
}

/**
 * Get pointer of the neoled.
 *
 * @return Neoled*
*/
Neoled *HardwareManager::get_neoled(void)
{
    if (m_neoled != nullptr)
    {
        return m_neoled;
    }

    // error?
    return nullptr;
}

/**
 * Get pointer of the neoled.
 *
 * @return Neoled*
*/
Led *HardwareManager::get_led(void)
{
    if (m_led != nullptr)
    {
        return m_led;
    }

    // error?
    return nullptr;
}


/**
 * Get pointer of sensor a.
 *
 * @return LegoSensor*
*/
LegoSensor *HardwareManager::get_sensor_a(void)
{
    return nullptr;
}

/**
 * Get pointer of sensor b.
 *
 * @return LegoSensor*
*/
LegoSensor *HardwareManager::get_sensor_b(void)
{
    return nullptr;
}

/**
 * Get pointer of sensor c.
 *
 * @return LegoSensor*
*/
LegoSensor *HardwareManager::get_sensor_c(void)
{
    return nullptr;
}

/**
 * Get pointer of sensor d.
 *
 * @return LegoSensor*
*/
LegoSensor *HardwareManager::get_sensor_d(void)
{
    return nullptr;
}

/**
 * Get pointer of the IMU.
 *
 * @return IMU*
*/
IMU *HardwareManager::get_imu(void)
{
    if (m_imu != nullptr)
    {
        return m_imu;
    }

    // error?
    return nullptr;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
