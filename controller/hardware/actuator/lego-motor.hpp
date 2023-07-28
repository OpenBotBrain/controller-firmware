#pragma once

#include <module/module-output.hpp>
#include <cstdint>

//  Number of average samples for each motor:
//  - NXT Motor     =
//  - Medium Motor  = 1, 2,  4,  8
//  - Large Motor   = 2, 8, 16, 32

//  Medium Motor has no jitter on the tacho while the Large Motor does because
//  it has one gear wheel less.
//
//  Medium Motor reaction time is much faster than Large Motor due to smaller motor
//  and smaller gearbox.

static constexpr uint16_t MAX_MOTOR_SPEED      = 1000;
static constexpr uint16_t DEFAULT_MOTOR_SPEED  = 500;
static constexpr uint16_t MIN_MOTOR_SPEED      = 0;

enum class Lego_Motor_Type
{
    UNDEFINED_LEGO_MOTOR = 0,
    EV3_LARGE_MOTOR,
    EV3_MEDIUM_MOTOR,
    NXT_MOTOR,

    LEGO_MOTOR_TYPE_TOTAL
};

class LegoMotor
{
    public:

        virtual void forward(int32_t rotation) = 0;

        virtual void backward(int32_t rotation) = 0;

        virtual void stop() = 0;

        virtual void start_sync(LegoMotor *motor) = 0;

        virtual void end_sync() = 0;

        virtual bool is_stalled() = 0;

        virtual int32_t get_tacho_count() = 0;

        virtual void set_motor_pwm(uint8_t motor_pwm) = 0;

        virtual uint8_t get_motor_pwm() = 0;

        virtual void set_motor_encoder(uint8_t motor_encoder) = 0;

        virtual uint8_t get_motor_encoder() = 0;

        virtual void set_motor_speed(uint32_t motor_speed) = 0;

        virtual uint32_t get_motor_speed() = 0;

    private:

        virtual void drive_motor(float speed, int32_t rotation) = 0;
};

Lego_Motor_Type get_lego_motor_type(OutputPort* port);
