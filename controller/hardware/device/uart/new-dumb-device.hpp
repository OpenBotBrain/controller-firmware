#pragma once

#include <device/lego-device.hpp>

class NewDumbDevice : public LegoDevice
{
    public:

        NewDumbDevice() {};

        virtual void init() override {};

        virtual void update() override {};
};
