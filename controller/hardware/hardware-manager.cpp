#include <hardware/hardware-manager.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init everything within the hardware manager.
*/
void HardwareManager::init()
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
 * Update everything within the hardware manager.
*/
void HardwareManager::update()
{

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
 * Get pointer of given actuator type.
 *
 * @return Actuator*
*/
LegoMotor* HardwareManager::get_lego_motor(Lego_Motor_Port actuator_type)
{
    uint8_t actuator = static_cast<uint8_t>(actuator_type);
    return m_lego_motors[actuator];
}

Neoled* HardwareManager::get_neoled()
{
    return m_neoled;
}

Led* HardwareManager::get_led()
{
    return m_led;
}

/**
 * Get pointer of given sensor type.
 *
 * @return Sensor*
*/
LegoSensor* HardwareManager::get_lego_sensor(Lego_Sensor_Port sensor_type)
{
    uint8_t sensor = static_cast<uint8_t>(sensor_type);
    return m_lego_sensors[sensor];
}

IMU* HardwareManager::get_imu()
{
    return m_imu;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
