#pragma once

#include <actuator/hardware-actuator.hpp>
#include <sensor/hardware-sensor.hpp>
#include <mutex>

static constexpr uint16_t ACTUATOR_COUNT = 5;
static constexpr uint16_t SENSOR_COUNT   = 5;

enum Acuator_Type
{
    ACTUATOR_A = 0,
    ACTUATOR_B,
    ACTUATOR_C,
    ACTUATOR_D,
    NEOLED
};

enum Sensor_Type
{
    SENSOR_1 = 0,
    SENSOR_2,
    SENSOR_3,
    SENSOR_4,
    IMU
};

class HardwareManager
{
    public:

        HardwareManager(void) {};

        void init(void);

        void update(void);

        Actuator *get_actuator(Acuator_Type actuator);

        Actuator *get_actuator_a(void);

        Actuator *get_actuator_b(void);

        Actuator *get_actuator_c(void);

        Actuator *get_actuator_d(void);

        Actuator *get_neoled(void);

        Sensor *get_sensor(Sensor_Type sensor);

        Sensor *get_sensor_1(void);

        Sensor *get_sensor_2(void);

        Sensor *get_sensor_3(void);

        Sensor *get_sensor_4(void);

        Sensor *get_imu(void);

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