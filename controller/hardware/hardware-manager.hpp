#pragma once

#include <actuator/hardware-actuator.hpp>
#include <actuator/lego-motor.hpp>
#include <led/board-neoled.hpp>
#include <led/board-led.hpp>
#include <sensor/hardware-sensor.hpp>
#include <sensor/lego-sensor.hpp>
#include <imu/board-imu.hpp>
#include <battery/board-battery.hpp>
#include <mutex>

static constexpr uint16_t PORT_COUNT = 4;

enum class Lego_Motor_Port
{
    ACTUATOR_A  = 0,
    ACTUATOR_B  = 1,
    ACTUATOR_C  = 2,
    ACTUATOR_D  = 3
};

enum class Lego_Sensor_Port
{
    SENSOR_1    = 0,
    SENSOR_2    = 1,
    SENSOR_3    = 2,
    SENSOR_4    = 3
};

struct Hardware_Config
{
    uint16_t neoled_update_interval;
    uint16_t led_update_interval;
    uint16_t imu_update_interval;
};

class HardwareManager
{
    public:

        HardwareManager() {};

        void init();

        void update();

        Hardware_Config get_hardware_config();

        LegoMotor* get_lego_motor(Lego_Motor_Port actuator);

        Neoled* get_neoled();

        Led* get_led();

        LegoSensor* get_lego_sensor(Lego_Sensor_Port sensor);

        IMU* get_imu();

    private:

        Hardware_Config m_hardware_config;

        std::array<LegoMotor* , PORT_COUNT> m_lego_motors =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        std::array<LegoSensor* , PORT_COUNT> m_lego_sensors =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        Neoled* m_neoled;

        Led* m_led;

        IMU* m_imu;
};
