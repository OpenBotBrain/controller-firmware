#pragma once

#include <sensor/hardware-sensor.hpp>

class IMU
{
    public:

        IMU() {};

        void init() {};

        void update() {};

        float fetch_sample();

        float* fetch_accel();

        float* fetch_gyro();

        float* fetch_roll_pitch();
};
