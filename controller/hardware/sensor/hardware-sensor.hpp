#pragma once

class Sensor
{
    public:

        virtual void init(void) = 0;

        virtual void update(void) = 0;

        virtual float fetch_sample(void) = 0;
};
