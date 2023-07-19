#include <motor/ev3-medium-motor.hpp>
#include <cstdint>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an EV3 Mediuum Motor.
 *
 * @param *port pointer to motors port.
 * @param motor_pmw Timer Type value for motor pwm.
 * @param motor_encoder Timer Type value for motor encoder.
*/
EV3MediumMotor::EV3MediumMotor(OutputPort* port, TimerType motor_pwm, TimerType motor_encoder)
{
    m_port = port;
    m_motor_pwm = motor_pwm;
    m_motor_encoder = motor_encoder;

    m_sync_motor = nullptr;

    m_synced = false;

    m_motor_speed = DEFAULT_MOTOR_SPEED;
}

/**
 * Drive the motor forward.
 *
 * @param rotation amount forward for the motor.
*/
void EV3MediumMotor::forward(int32_t rotation)
{
    if (rotation == 0)
    {
        return;
    }

    if (rotation < 0)
    {
        backward(-rotation);
        return;
    }

    return;
}

/**
 * Drive the motor backward.
 *
 * @param rotation amount backward for the motor.
*/
void EV3MediumMotor::backward(int32_t rotation)
{
    if (rotation == 0)
    {
        return;
    }

    if (rotation < 0)
    {
        forward(-rotation);
        return;
    }

    return;
}

/**
 * Stop the motor.
*/
void EV3MediumMotor::stop()
{
    return;
}

/**
 * Sync with another Medium Motor.
 *
 * @param motor address of other motor that is being synced with.
*/
void EV3MediumMotor::start_sync(EV3MediumMotor* motor)
{
    if (m_synced)
    {
        return;
    }

    stop();
    motor->stop();

    m_sync_motor = motor;
    m_synced = true;

    return;
}

/**
 * Stops syncing the motors if is synced.
*/
void EV3MediumMotor::end_sync()
{
    if (m_sync_motor == nullptr)
    {
        return;
    }

    m_sync_motor = nullptr;
    m_synced = false;

    return;
}

/**
 * Checks if the motor is currently stalled.
 *
 * @return boolean - stalled value.
*/
bool EV3MediumMotor::is_stalled()
{
    return false;
}

/**
 * Get the tacho count of the motor.
 * Uses the encoder of the motor.
 *
 * @return int32_t - tacho count.
*/
int32_t EV3MediumMotor::get_tacho_count()
{
    return 0;
}

/**
 * Set the motor pwm
 *
 * @param motor_pwm
*/
void EV3MediumMotor::set_motor_pwm(uint8_t motor_pwm)
{
    m_motor_pwm = static_cast<TimerType>(motor_pwm);
}

/**
 * Get the motor pwm
 *
 * @return motor_pwm
*/
uint8_t EV3MediumMotor::get_motor_pwm()
{
    return static_cast<uint8_t>(m_motor_pwm);
}

/**
 * Set the motor encoder
 *
 * @param motor_encoder
*/
void EV3MediumMotor::set_motor_encoder(uint8_t motor_encoder)
{
    m_motor_encoder = static_cast<TimerType>(motor_encoder);
}


/**
 * Get the motor encoder
 *
 * @return motor_encoder
*/
uint8_t EV3MediumMotor::get_motor_encoder()
{
    return static_cast<uint8_t>(m_motor_encoder);
}

/**
 * Set the motor speed.
 *
 * If motor speed is greater than the maximum motor speed then it is set to the max speed.
 *
 * @param motor_speed
*/
void EV3MediumMotor::set_motor_speed(uint32_t motor_speed)
{
    m_motor_speed = (motor_speed > MAX_MOTOR_SPEED) ? MAX_MOTOR_SPEED : motor_speed;
    m_motor_speed = (motor_speed < MIN_MOTOR_SPEED) ? MIN_MOTOR_SPEED : motor_speed;
}

/**
 * Get the motor speed.
 *
 * @return motor_speed
*/
uint32_t EV3MediumMotor::get_motor_speed(void)
{
    return m_motor_speed;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------

/**
 * Drive a motor.
 *
 * @param speed float value containing speed of motor.
 * @param rotation amount of rotation of the motor.
 * @param immediate_return should we immediately return to program after telling the motor to run?
*/
void EV3MediumMotor::drive_motor(float speed, int32_t rotation, bool immediate_return)
{
    if (immediate_return);

    if (speed);

    if (rotation);
}
