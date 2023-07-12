#pragma once

#include <sensor/hardware-sensor.hpp>

class IMU : public Sensor
{
    public:

        IMU(void) {};

        void init(void);

        void update(void);

        float fetch_sample(void);

        float* fetch_accel(void);

        float* fetch_gyro(void);

        float* fetch_roll_pitch(void);
};
