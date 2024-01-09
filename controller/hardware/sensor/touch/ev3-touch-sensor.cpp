#include <touch/ev3-touch-sensor.hpp>
#include <hardware/hardware-config.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an instance of EV3 Touch Sensor.
 *
 * sensor mode is defaulted to UNDEFINED when one is not given.
 *
 * @param *port pointer to Input port.
 */
EV3TouchSensor::EV3TouchSensor(Lego_Sensor_Port sensor_port)
{
    m_sensor_port = sensor_port;
    m_sensor_mode = EV3_Touch_Sensor_Mode::UNDEFINED;

    m_input_port = &module_port_get_input_port((uint8_t)sensor_port);
}

/**
 * Create an instance of EV3 Touch Sensor.
 *
 * @param *port pointer to Input port.
 * @param sensor_mode set the sensor mode here.
 */
EV3TouchSensor::EV3TouchSensor(Lego_Sensor_Port sensor_port, EV3_Touch_Sensor_Mode sensor_mode)
{
    m_sensor_port = sensor_port;
    m_sensor_mode = sensor_mode;

    m_input_port = &module_port_get_input_port((uint8_t)sensor_port);
}

/**
 * Initialise the EV3 Touch Sensor.
 *
 * Init the port that the Sensor is using. Set the EV3 Sensor Mode to TOUCH.
 */
void EV3TouchSensor::init()
{
    m_input_port->init();
    m_input_port->set_mode(InputPort::Mode::INPUT);
    m_sensor_mode = EV3_Touch_Sensor_Mode::TOUCH;
}

/**
 * Update the EV3 Touch Sensor.
 */
void EV3TouchSensor::update()
{
    m_voltage = m_input_port->get_voltage_v(InputPort::PinID::PIN1);
    m_pressed = (m_voltage >= 0.4f);
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
    return m_voltage;
}

/**
 * Fetch whether the button is being pressed down.
 *
 * @return is the button pressed?
*/
bool EV3TouchSensor::is_pressed()
{
    return m_pressed;
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
