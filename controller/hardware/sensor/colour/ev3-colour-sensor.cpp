#include <colour/ev3-colour-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of EV3 Colour Sensor.
 *
 * Sensor mode is defaulted to WHITE when one is not given.
 *
 * @param *port pointer to Input Port.
*/
EV3ColourSensor::EV3ColourSensor(InputPort *port)
{
    m_port = port;
    m_sensor_mode = EV3_COLOUR_SENSOR_WHITE;
}

/**
 * Create an instance of EV3 Colour Sensor.
 *
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
*/
EV3ColourSensor::EV3ColourSensor(InputPort *port, EV3_Colour_Sensor_Mode sensor_mode)
{
    m_port = port;
    m_sensor_mode = sensor_mode;
}

/**
 * Fetch a sample from the EV3 Colour Sensor.
 *
 * @param *sample pointer to float value that stores the sample.
*/
float EV3ColourSensor::fetch_sample()
{
    return 1.0f;
}

/**
 * Set the sensor mode of the EV3 Colour Sensor.
 *
 * @param sensor_mode
*/
void EV3ColourSensor::set_mode(uint8_t sensor_mode)
{
    m_sensor_mode = static_cast<EV3_Colour_Sensor_Mode>(sensor_mode);
}

/**
 * Get the sensor mode of the EV3 Colour Sensor.
 *
 * @return sensor_mode
*/
uint8_t EV3ColourSensor::get_mode()
{
    return static_cast<uint8_t>(m_sensor_mode);
}
