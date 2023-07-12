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
enum EV3_Sound_Sensor_Mode
{
    EV3_SOUND_SENSOR_UNDEFINED = 0,
    EV3_SOUND_SENSOR_DB,
    EV3_SOUND_SENSOR_DBA
};

class EV3SoundSensor : public LegoSensor
{
    public:

        EV3SoundSensor(InputPort *port);

        EV3SoundSensor(InputPort *port, EV3_Sound_Sensor_Mode sensor_mode);

        float fetch_sample(void);

        void set_mode(uint8_t sensor_mode);

        uint8_t get_mode(void);

    private:

        InputPort *m_port;

        EV3_Sound_Sensor_Mode m_sensor_mode;
};
