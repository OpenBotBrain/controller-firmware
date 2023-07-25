#include <motor/nxt-motor.hpp>
#include <cstdint>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Create an NXT Motor.
 *
 * @param *port pointer to motors port.
 * @param motor_pmw Timer Type value for motor pwm.
 * @param motor_encoder Timer Type value for motor encoder.
*/
NXTMotor::NXTMotor(OutputPort* port, TimerType motor_pwm, TimerType motor_encoder)
{
    m_port = port;
    m_motor_pwm = motor_pwm;
    m_motor_encoder = motor_encoder;

    m_sync_motor = nullptr;

    m_synced = false;
}

/**
 * Drive the motor forward.
 *
 * @param rotation amount forward for the motor.
*/
void NXTMotor::forward(int32_t rotation)
{
    drive_motor(0.0f, rotation);

    return;
}

/**
 * Drive the motor backward.
 *
 * @param rotation amount backward for the motor.
*/
void NXTMotor::backward(int32_t rotation)
{
    drive_motor(0.0f, -rotation);

    return;
}

/**
 * Stop the motor.
*/
void NXTMotor::stop()
{
    return;
}

/**
 * Sync with another Motor.
 *
 * @param motor address of other motor that is being synced with.
*/
void NXTMotor::start_sync(LegoMotor* motor)
{
    if (m_synced)
    {
        // sync must end before syncing again?
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
void NXTMotor::end_sync()
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
bool NXTMotor::is_stalled()
{
    return false;
}

/**
 * Get the tacho count of the motor.
 * Uses the encoder of the motor.
 *
 * @return int32_t - tacho count.
*/
int32_t NXTMotor::get_tacho_count()
{
    return 0;
}

/**
 * Set the motor pwm
 *
 * @param motor_pwm
*/
void NXTMotor::set_motor_pwm(uint8_t motor_pwm)
{
    m_motor_pwm = static_cast<TimerType>(motor_pwm);
}

/**
 * Get the motor pwm
 *
 * @return motor_pwm
*/
uint8_t NXTMotor::get_motor_pwm()
{
    return static_cast<uint8_t>(m_motor_pwm);
}

/**
 * Set the motor encoder
 *
 * @param motor_encoder
*/
void NXTMotor::set_motor_encoder(uint8_t motor_encoder)
{
    m_motor_encoder = static_cast<TimerType>(motor_encoder);
}


/**
 * Get the motor encoder
 *
 * @return motor_encoder
*/
uint8_t NXTMotor::get_motor_encoder()
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
void NXTMotor::set_motor_speed(uint32_t motor_speed)
{
    m_motor_speed = (motor_speed > MAX_MOTOR_SPEED) ? MAX_MOTOR_SPEED : motor_speed;
}

/**
 * Get the motor speed.
 *
 * @return motor_speed
*/
uint32_t NXTMotor::get_motor_speed(void)
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
void NXTMotor::drive_motor(float speed, int32_t rotation)
{
    if (speed);

    if (rotation);
}
