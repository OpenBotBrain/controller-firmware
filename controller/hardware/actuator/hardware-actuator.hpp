#pragma once

class Actuator
{
    public:

        Actuator(void) {};

        virtual void init(void) = 0;

        virtual void update(void) = 0;
};
