#pragma once

class Sensor
{
    public:

        virtual void init() = 0;

        virtual void update() = 0;

        virtual float fetch_sample() = 0;
};
