#pragma once

#include <sensor/hardware-sensor.hpp>

class IMU : public Sensor
{
    public:

        IMU() {};

        virtual void init() override {};

        virtual void update() override {};

        virtual float fetch_sample() override;

        float* fetch_accel();

        float* fetch_gyro();

        float* fetch_roll_pitch();
};
