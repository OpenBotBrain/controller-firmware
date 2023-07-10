#pragma once

#include <sensor/hardware-sensor.hpp>

class IMU : public Sensor
{
    public:

        IMU(void);

        void init(void);

        void update(void);

        float fetch_sample(void);

    private:
    
};
