#pragma once

#include <actuator/hardware-actuator.hpp>
#include <actuator/lego-motor.hpp>
#include <actuator/board-neoled.hpp>
#include <actuator/board-led.hpp>
#include <sensor/hardware-sensor.hpp>
#include <sensor/lego-sensor.hpp>
#include <sensor/board-imu.hpp>

class HardwareManager
{
    public:

        HardwareManager(void);

        void init(void);

        void update(void);

        LegoMotor *get_actuator_a(void);

        LegoMotor *get_actuator_b(void);

        LegoMotor *get_actuator_c(void);

        LegoMotor *get_actuator_d(void);

        Neoled *get_neoled(void);

        Led *get_led(void);

        LegoSensor *get_sensor_a(void);

        LegoSensor *get_sensor_b(void);

        LegoSensor *get_sensor_c(void);

        LegoSensor *get_sensor_d(void);

        IMU *get_imu(void);

    private:

        LegoMotor *m_actuator_a;

        LegoMotor *m_actuator_b;

        LegoMotor *m_actuator_c;

        LegoMotor *m_actuator_d;

        Neoled *m_neoled;

        Led *m_led;

        LegoSensor *m_sensor_a;

        LegoSensor *m_sensor_b;

        LegoSensor *m_sensor_c;

        LegoSensor *m_sensor_d;

        IMU *m_imu;
};
