#include <touch/nxt-touch-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of NXT Touch Sensor.
 *
 * sensor mode is defaulted to DB when one is not given.
 *
 * @param *port pointer to Input port.
*/
NXTTouchSensor::NXTTouchSensor(InputPort* port)
{
    m_port = port;
    m_sensor_mode = NXT_Touch_Sensor_Mode::TOUCH;
}

/**
 * Create an instance of NXT Touch Sensor.
 *
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
*/
NXTTouchSensor::NXTTouchSensor(InputPort* port, NXT_Touch_Sensor_Mode sensor_mode)
{
    m_port = port;
    m_sensor_mode = sensor_mode;
}

/**
 * Fetch a sample from the NXT Touch Sensor.
 *
 * Either 0.0f for off, 1.0f for on.
 *
 * @param *sample pointer to float value that stores the sample.
*/
float NXTTouchSensor::fetch_sample()
{
    return 1.0f;
}

/**
 * Set the sensor mode of the NXT Touch Sensor.
 *
 * @param sensor_mode
*/
void NXTTouchSensor::set_mode(uint8_t sensor_mode)
{
    m_sensor_mode = static_cast<NXT_Touch_Sensor_Mode>(sensor_mode);
}

/**
 * Get the sensor mode of the NXT Touch Sensor.
 *
 * @return sensor_mode
*/
uint8_t NXTTouchSensor::get_mode()
{
    return static_cast<int8_t>(m_sensor_mode);
}
