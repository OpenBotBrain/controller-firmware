#pragma once

#include <actuator/lego-motor.hpp>
#include <module/module-output.hpp>
#include <stm-hal/hal-tim.hpp>

class EV3MediumMotor : public LegoMotor
{
    public:

        EV3MediumMotor(OutputPort *port, TimerType motor_pwm, TimerType motor_encoder);

        void forward(int32_t rotation) override;

        void backward(int32_t rotation) override;

        void stop(void) override;

        void start_sync(EV3MediumMotor *motor);

        void end_sync(void) override;

        bool is_stalled(void) override;

        int32_t get_tacho_count(void) override;

        void set_motor_pwm(uint8_t motor_pwm) override;

        uint8_t get_motor_pwm(void) override;

        void set_motor_encoder(uint8_t motor_encoder) override;

        uint8_t get_motor_encoder(void) override;

        void set_motor_speed(uint32_t motor_speed) override;

        uint32_t get_motor_speed(void) override;

    private:

        void drive_motor(float speed, int32_t rotation, bool immediate_return) override;

    private:

        OutputPort *m_port;

        TimerType m_motor_pwm;

        TimerType m_motor_encoder;

        EV3MediumMotor *m_sync_motor;

        bool m_synced;

        uint32_t m_motor_speed;
};
