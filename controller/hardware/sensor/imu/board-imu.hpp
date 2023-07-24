#pragma once

#include <sensor/hardware-sensor.hpp>
#include <cstdint>

static constexpr uint8_t ACCEL_SIZE = 3;
static constexpr uint8_t GYRO_SIZE  = 3;
static constexpr uint8_t ROLL_SIZE  = 2;

struct IMU_data
{
    float accel[ACCEL_SIZE];
    float gyro[GYRO_SIZE];
    float roll[ROLL_SIZE];
};

class IMU : public Sensor
{
    public:

        IMU() {};

        virtual void init() override;

        virtual void update() override;

        IMU_data get_imu_data();

    private:

        IMU_data m_imu_data;
};
