#pragma once

#include <actuator/hardware-actuator.hpp>
#include <actuator/lego-motor.hpp>
#include <led/board-neoled.hpp>
#include <led/board-led.hpp>
#include <sensor/hardware-sensor.hpp>
#include <mutex>

struct Hardware_Config
{
    uint16_t neoled_update_interval;
    uint16_t led_update_interval;
    uint16_t imu_update_interval;
};

class HardwareManager
{
    public:

        HardwareManager(void) {};

        void init(void);

        void update(void);

        Actuator *get_actuator(Acuator_Type actuator);

        void update(void);

        Hardware_Config get_hardware_config();

        LegoMotor *get_actuator_a(void);

        LegoMotor *get_actuator_b(void);

        LegoMotor *get_actuator_c(void);

        Sensor *get_sensor(Sensor_Type sensor);

        Sensor *get_sensor_1(void);

        Sensor *get_sensor_2(void);

        Sensor *get_sensor_3(void);

        Sensor *get_sensor_4(void);

        LegoSensor *get_sensor_b(void);

        LegoSensor *get_sensor_c(void);

        LegoSensor *get_sensor_d(void);

        IMU *get_imu(void);

        Battery *get_battery(void);

    private:

        std::array<Actuator* , ACTUATOR_COUNT> m_actuators =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        std::array<Sensor* , ACTUATOR_COUNT> m_sensors =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };
};
