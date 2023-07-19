#include <hardware/hardware-manager.hpp>

void HardwareManager::init()
{

}

void HardwareManager::update()
{

}

Actuator *HardwareManager::get_actuator(Acuator_Type actuator)
{
    return m_actuators[actuator];
}

Actuator *HardwareManager::get_actuator_a(void)
{
    return m_actuators[0];
}

Actuator *HardwareManager::get_actuator_b(void)
{
    return m_actuators[1];
}

Actuator *HardwareManager::get_actuator_c(void)
{
    return m_actuators[2];
}

Actuator *HardwareManager::get_actuator_d(void)
{
    return m_actuators[3];
}

Actuator *HardwareManager::get_neoled(void)
{
    return m_actuators[4];
}

Sensor *HardwareManager::get_sensor(Sensor_Type sensor)
{
    return m_sensors[sensor];
}

Sensor *HardwareManager::get_sensor_1(void)
{
    return m_sensors[0];
}

Sensor *HardwareManager::get_sensor_2(void)
{
    return m_sensors[1];
}

Sensor *HardwareManager::get_sensor_3(void)
{
    return m_sensors[2];
}

Sensor *HardwareManager::get_sensor_4(void)
{
    return m_sensors[3];
}

Sensor *HardwareManager::get_imu(void)
{
    return m_sensors[4];
}
