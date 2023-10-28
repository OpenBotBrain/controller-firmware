#include <sound/nxt-sound-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of NXT Sound Sensor.
 *
 * Sensor mode is defaulted to DB when one is not given.
 *
 * @param *port pointer to Input port.
*/
NXTSoundSensor::NXTSoundSensor(InputPort* port)
{
    m_port = port;
    m_sensor_mode = NXT_Sound_Sensor_Mode::DB;
}

/**
 * Create an instance of NXT Sound Sensor.
 *
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
*/
NXTSoundSensor::NXTSoundSensor(InputPort* port, NXT_Sound_Sensor_Mode sensor_mode)
{
    m_port = port;
    m_sensor_mode = sensor_mode;
}

/**
 * Fetch a sample from the NXT Sound Sensor.
 *
 * @param *sample pointer to float value that stores the sample.
*/
float NXTSoundSensor::fetch_sound_sample()
{
    return 1.0f;
}

/**
 * Set the sensor mode of the NXT Sound Sensor.
 *
 * @param sensor_mode
*/
void NXTSoundSensor::set_mode(uint8_t sensor_mode)
{
    m_sensor_mode = static_cast<NXT_Sound_Sensor_Mode>(sensor_mode);
}

/**
 * Get the sensor mode of the NXT Sound Sensor.
 *
 * @return sensor_mode
*/
uint8_t NXTSoundSensor::get_mode()
{
    return static_cast<int8_t>(m_sensor_mode);
}
