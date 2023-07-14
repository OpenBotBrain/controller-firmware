#pragma once

#include <actuator/hardware-actuator.hpp>
#include <actuator/lego-motor.hpp>
#include <led/board-neoled.hpp>
#include <led/board-led.hpp>
#include <sensor/hardware-sensor.hpp>
#include <sensor/lego-sensor.hpp>
#include <imu/board-imu.hpp>

struct Hardware_Config
{
    uint16_t neoled_update_interval;
    uint16_t led_update_interval;
    uint16_t imu_update_interval;
};

class HardwareManager
{
    public:

        HardwareManager(void);

        void init(void);

        void update(void);

        Hardware_Config get_hardware_config();

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

        Hardware_Config m_hardware_config;

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
