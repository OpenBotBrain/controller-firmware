#pragma once

class Device
{
    public:

        Device(void) {};

        virtual void init(void) = 0;

        virtual void update(void) = 0;
};