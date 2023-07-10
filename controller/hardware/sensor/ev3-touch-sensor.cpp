#include <sensor/ev3-touch-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of EV3 Touch Sensor.
 * 
 * sensor mode is defaulted to DB when one is not given.
 * 
 * @param *port pointer to Input port.
*/
EV3TouchSensor::EV3TouchSensor(InputPort *port)
{
    m_port = port;
    m_sensor_mode = EV3_TOUCH_SENSOR_TOUCH;
}

/**
 * Create an instance of EV3 Touch Sensor.
 * 
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
*/
EV3TouchSensor::EV3TouchSensor(InputPort *port, EV3_Touch_Sensor_Mode sensor_mode)
{
    m_port = port;
    m_sensor_mode = sensor_mode;
}

/**
 * Fetch a sample from the EV3 Touch Sensor.
 * 
 * Either 0.0f for off, 1.0f for on.
 * 
 * @param *sample pointer to float value that stores the sample.
*/
float EV3TouchSensor::fetch_sample()
{
    return 1.0f;
}

/**
 * Set the sensor mode of the EV3 Touch Sensor.
 * 
 * @param sensor_mode
*/
void EV3TouchSensor::set_mode(uint8_t sensor_mode)
{
    m_sensor_mode = static_cast<EV3_Touch_Sensor_Mode>(sensor_mode);
}

/**
 * Get the sensor mode of the EV3 Touch Sensor.
 * 
 * @return sensor_mode
*/
uint8_t EV3TouchSensor::get_mode()
{
    return static_cast<int8_t>(m_sensor_mode);
}
