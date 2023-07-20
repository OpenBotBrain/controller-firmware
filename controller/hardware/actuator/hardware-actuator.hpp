#pragma once

class Actuator
{
    public:

        virtual void init() = 0;

        virtual void update() = 0;
};
