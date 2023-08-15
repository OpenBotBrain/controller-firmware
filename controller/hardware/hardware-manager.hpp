#pragma once

#include <actuator/hardware-actuator.hpp>
#include <actuator/lego-motor.hpp>
#include <led/board-neoled.hpp>
#include <led/board-led.hpp>
#include <sensor/hardware-sensor.hpp>
#include <sensor/lego-sensor.hpp>
#include <imu/board-imu.hpp>
#include <battery/board-battery.hpp>
#include <button/board-buttons.hpp>
#include <mutex>

static constexpr uint16_t PORT_COUNT = 4;

enum class Lego_Motor_Port
{
    ACTUATOR_A = 0,
    ACTUATOR_B,
    ACTUATOR_C,
    ACTUATOR_D
};

enum class Lego_Sensor_Port
{
    SENSOR_1 = 0,
    SENSOR_2,
    SENSOR_3,
    SENSOR_4
};

struct Hardware_Config
{
    uint16_t neoled_update_interval;
    uint16_t led_update_interval;
    uint16_t imu_update_interval;
    uint16_t button_update_interval;
    uint16_t battery_update_interval;
};

class HardwareManager
{
    public:

        HardwareManager() {};

        void init();

        void update();

        Hardware_Config get_hardware_config();

        LegoMotor* get_lego_motor(Lego_Motor_Port actuator);

        LegoSensor* get_lego_sensor(Lego_Sensor_Port sensor);

        NeoLED& get_neoled();

        LED& get_led();

        IMU& get_imu();

        Buttons& get_buttons();

        Battery& get_battery();

    private:

        Hardware_Config m_hardware_config;

        std::array<LegoMotor*, PORT_COUNT> m_lego_motors =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        std::array<LegoSensor*, PORT_COUNT> m_lego_sensors =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        NeoLED m_neoled;

        LED m_led;

        IMU m_imu;

        Buttons m_buttons;

        Battery m_battery;
};
