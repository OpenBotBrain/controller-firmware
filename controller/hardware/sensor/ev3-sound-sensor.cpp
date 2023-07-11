#include <sensor/ev3-sound-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of EV3 Sound Sensor.
 * 
 * sensor mode is defaulted to DB when one is not given.
 * 
 * @param *port pointer to Input port.
*/
EV3SoundSensor::EV3SoundSensor(InputPort *port)
{
    m_port = port;
    m_sensor_mode = EV3_SOUND_SENSOR_DB;
}

/**
 * Create an instance of EV3 Sound Sensor.
 * 
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
*/
EV3SoundSensor::EV3SoundSensor(InputPort *port, EV3_Sound_Sensor_Mode sensor_mode)
{
    m_port = port;
    m_sensor_mode = sensor_mode;
}

/**
 * Fetch a sample from the EV3 Sound Sensor.
 * 
 * @param *sample pointer to float value that stores the sample.
*/
float EV3SoundSensor::fetch_sample()
{
    return 1.0f;
}

/**
 * Set the sensor mode of the EV3 Sound Sensor.
 * 
 * @param sensor_mode
*/
void EV3SoundSensor::set_mode(uint8_t sensor_mode)
{
    m_sensor_mode = static_cast<EV3_Sound_Sensor_Mode>(sensor_mode);
}

/**
 * Get the sensor mode of the EV3 Sound Sensor.
 * 
 * @return sensor_mode
*/
uint8_t EV3SoundSensor::get_mode()
{
    return static_cast<int8_t>(m_sensor_mode);
}
