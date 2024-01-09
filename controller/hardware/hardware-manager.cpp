#include <hardware/hardware-manager.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init everything within the hardware manager.
*/
void HardwareManager::init()
{
    NeoLED m_neoled;

    LED m_led;

    IMU m_imu;

    m_hardware_config = Hardware_Config
    {
        .neoled_update_interval = 10,
        .led_update_interval = 250,
        .imu_update_interval = 5,
        .button_update_interval = 5,
        .sensor_update_interval = 5,
        .actuator_update_interval = 5,
    };
}

/**
 * Update everything within the hardware manager.
*/
void HardwareManager::update()
{
    // will update all devices within the hardware manager.
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

/**
 * Returns a reference to Neoled.
 *
 * @return Neoled&
*/
NeoLED& HardwareManager::get_neoled()
{
    return m_neoled;
}

/**
 * Returns a reference to Led.
 *
 * @return Led&
*/
LED& HardwareManager::get_led()
{
    return m_led;
}

/**
 * Returns a reference to IMU.
 *
 * @return IMU&
*/
IMU& HardwareManager::get_imu()
{
    return m_imu;
}

/**
 * Returns a reference to Buttons.
 *
 * @return IMU&
*/
Buttons& HardwareManager::get_buttons()
{
    return m_buttons;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
