#include <touch/nxt-touch-sensor.hpp>
#include <hardware/hardware-config.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of NXT Touch Sensor.
 *
 * sensor mode is defaulted to UNDEFINED when one is not given.
 *
 * @param *port pointer to Input port.
 */
NXTTouchSensor::NXTTouchSensor(Lego_Sensor_Port sensor_port)
{
    m_sensor_port = sensor_port;
    m_sensor_mode = NXT_Touch_Sensor_Mode::UNDEFINED;

    m_input_port = &module_port_get_input_port((uint8_t)sensor_port);
}

/**
 * Create an instance of NXT Touch Sensor.
 *
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
 */
NXTTouchSensor::NXTTouchSensor(Lego_Sensor_Port sensor_port, NXT_Touch_Sensor_Mode sensor_mode)
{
    m_sensor_port = sensor_port;
    m_sensor_mode = sensor_mode;

    m_input_port = &module_port_get_input_port((uint8_t)sensor_port);
}

/**
 * Initialise the NXT Touch Sensor.
 *
 * Init the port that the Sensor is using. Set the NXT Sensor Mode to TOUCH.
 */
void NXTTouchSensor::init()
{
    m_input_port->init();
    m_input_port->set_mode(InputPort::Mode::INPUT);
    m_sensor_mode = NXT_Touch_Sensor_Mode::TOUCH;
}

/**
 * Update the NXT Touch Sensor.
 */
void NXTTouchSensor::update()
{
    m_voltage = m_input_port->get_voltage_v(InputPort::PinID::PIN1);
    m_pressed = (m_voltage <= 1.0f);
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
    return m_voltage;
}

/**
 * Fetch whether the button is being pressed down.
 *
 * @return is the button pressed?
*/
bool NXTTouchSensor::is_pressed()
{
    return m_pressed;
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
