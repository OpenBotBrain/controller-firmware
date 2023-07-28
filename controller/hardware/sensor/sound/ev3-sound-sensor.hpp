#pragma once

#include <sensor/lego-sensor.hpp>
#include <module/module-input.hpp>
#include <cstdint>

/**
 * EV3 Sound Sensor modes.
 *
 * - EV3_SOUND_SENSOR_UNDEFINED: undefined mode.
 * - EV3_SOUND_SENSOR_DB: db mode.
 * - EV3_SOUND_SENSOR_DBA: dba mode, closer to human perception.
*/
enum class EV3_Sound_Sensor_Mode
{
    UNDEFINED = 0,
    DB,
    DBA
};

class EV3SoundSensor : public LegoSensor
{
    public:

        EV3SoundSensor(InputPort* port);

        EV3SoundSensor(InputPort* port, EV3_Sound_Sensor_Mode sensor_mode);

        virtual void init() override {};

        virtual void update() override {};

        virtual void set_mode(uint8_t sensor_mode) override;

        virtual uint8_t get_mode() override;

        float fetch_sound_sample();

    private:

        InputPort* m_port;

        EV3_Sound_Sensor_Mode m_sensor_mode;
};
