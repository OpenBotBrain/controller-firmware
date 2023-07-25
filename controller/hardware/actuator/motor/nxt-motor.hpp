#pragma once

#include <actuator/lego-motor.hpp>
#include <module/module-output.hpp>
#include <stm-hal/hal-tim.hpp>

class NXTMotor : public LegoMotor
{
    public:

        NXTMotor(OutputPort* port, TimerType motor_pwm, TimerType motor_encoder);

        virtual void forward(int32_t rotation) override;

        virtual void backward(int32_t rotation) override;

        virtual void stop() override;

        virtual void start_sync(LegoMotor* motor) override;

        virtual void end_sync() override;

        virtual bool is_stalled();

        virtual int32_t get_tacho_count() override;

        virtual void set_motor_pwm(uint8_t motor_pwm) override;

        virtual uint8_t get_motor_pwm() override;

        virtual void set_motor_encoder(uint8_t motor_encoder) override;

        virtual uint8_t get_motor_encoder() override;

        virtual void set_motor_speed(uint32_t motor_speed) override;

        virtual uint32_t get_motor_speed() override;

    private:

        virtual void drive_motor(float speed, int32_t rotation) override;

    private:

        OutputPort* m_port;

        TimerType m_motor_pwm;

        TimerType m_motor_encoder;

        LegoMotor* m_sync_motor;

        bool m_synced;

        uint32_t m_motor_speed;
};
