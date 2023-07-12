#pragma once

#include <actuator/lego-motor.hpp>
#include <module/module-output.hpp>
#include <stm-hal/hal-tim.hpp>

class NXTMotor : public LegoMotor
{
    public:

        NXTMotor(OutputPort *port, TimerType motor_pwm, TimerType motor_encoder);

        void forward(int32_t rotation);

        void backward(int32_t rotation);

        void stop(void);

        void start_sync(NXTMotor *motor);

        void end_sync(void);

        bool is_stalled(void);

        int32_t get_tacho_count(void);

        void set_motor_pwm(uint8_t motor_pwm);

        uint8_t get_motor_pwm(void);

        void set_motor_encoder(uint8_t motor_encoder);

        uint8_t get_motor_encoder(void);

        void set_motor_speed(uint32_t motor_speed);

        uint32_t get_motor_speed(void);

    private:

        void drive_motor(float speed, int32_t rotation, bool immediate_return);

    private:

        OutputPort *m_port;

        TimerType m_motor_pwm;

        TimerType m_motor_encoder;

        NXTMotor *m_sync_motor;

        bool m_synced;

        uint32_t m_motor_speed;
};
