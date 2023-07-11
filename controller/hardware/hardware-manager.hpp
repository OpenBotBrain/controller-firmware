#pragma once

#include <actuator/hardware-actuator.hpp>
#include <sensor/hardware-sensor.hpp>
#include <mutex>

class HardwareManager
{
    public:

        HardwareManager(void);

        ~HardwareManager(void);

        Actuator *get_actuator_a(void);

        Actuator *get_actuator_b(void);

        Actuator *get_actuator_c(void);

        Actuator *get_actuator_d(void);

        Actuator *get_neoled(void);

        Sensor *get_sensor_a(void);

        Sensor *get_sensor_b(void);

        Sensor *get_sensor_c(void);

        Sensor *get_sensor_d(void);

        Sensor *get_imu(void);

    private:

        Actuator *m_actuator_a;

        Actuator *m_actuator_b;
        
        Actuator *m_actuator_c;

        Actuator *m_actuator_d;        

        Actuator *m_neoled;

        Sensor *m_sensor_a;

        Sensor *m_sensor_b;
        
        Sensor *m_sensor_c;

        Sensor *m_sensor_d;        

        Sensor *m_imu;
};