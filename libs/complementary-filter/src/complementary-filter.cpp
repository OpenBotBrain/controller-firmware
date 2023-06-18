#include <complementary-filter/complementary-filter.hpp>
#include <math.h>

ComplementaryFilter::ComplementaryFilter()
{

}

void ComplementaryFilter::integrate_value_update(float* gyro_dps, float period_s)
{
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        m_integrated_gyro_rad[i] += DEG_TO_RAD * (gyro_dps[i] * period_s);

        if (m_integrated_gyro_rad[i] > PI)
        {
            m_integrated_gyro_rad[i] -= 2*PI;
        }
        else if (m_integrated_gyro_rad[i] < -PI)
        {
            m_integrated_gyro_rad[i] += 2*PI;
        }
    }
}

float ComplementaryFilter::get_module(float* data)
{
    float sum = 0;
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        sum += data[i] * data[i];
    }
    return std::sqrt(sum);
}

void ComplementaryFilter::gravity_magnitude_update(float new_val)
{
    static constexpr float GRAVITY_POLE = 0.999f;
    m_gravity_magnitude_m2s = GRAVITY_POLE * (m_gravity_magnitude_m2s) + ( 1.0f - GRAVITY_POLE ) * new_val;
}

void ComplementaryFilter::angular_velocity_update(float* gyro_dps)
{
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        m_angular_speed_rad_s[i] = gyro_dps[i] * DEG_TO_RAD;
    }
}

float ComplementaryFilter::get_filter_pole(float accel_magnitude)
{
    float k = FILTER_ACCEL_RELIABLE_POLE;

    m_omega_magnitude = get_module(m_angular_speed_rad_s);

    // The readings of the accelerometer are noisy due to general movement.
    // We want to trust less the accelerometer when it's magnitude is greater than
    // it should be in a steady position.
    // Also, a large angular velocity produces centrifugal forces in the accelerometer
    // if not placed along the axis of rotation.
    if (accel_magnitude > (m_gravity_magnitude_m2s + 0.1f) ||
        m_omega_magnitude > 0.2f)
    {
        m_counter = 10;
    }

    if (m_counter > 0)
    {
        k = FILTER_ACCEL_NON_RELIABLE_POLE;
        m_counter--;
    }

    return k;
}

void ComplementaryFilter::init()
{
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        m_integrated_gyro_rad[i] = 0.0f;
        m_angular_speed_rad_s[i] = 0.0f;
    }
}

void ComplementaryFilter::update(float* accel_m2s, float* gyro_dps, float dt_s)
{
    // Calculate the position from the integration of the IMU gyro
    integrate_value_update(gyro_dps, dt_s);

    // Update the value of the local G module
    float accel_module = get_module(accel_m2s);
    gravity_magnitude_update(accel_module);

    // Estimate the angular in rad/s
    angular_velocity_update(gyro_dps);

    // Get the current pole that we need to use depending on system noise
    float k = get_filter_pole(accel_module);

    // Calculate roll and pitch
    float pitch_accel = std::atan2(accel_m2s[0], accel_m2s[2]);
    float roll_accel = std::atan2(accel_m2s[1], accel_m2s[2]);

    // The estimated angles are a weighted average between the gyro and the accelerometer readings
    m_roll_rad = k * (m_roll_rad + m_angular_speed_rad_s[0] * dt_s) + (1.0f - k) * roll_accel;
    m_pitch_rad = k * (m_pitch_rad - m_angular_speed_rad_s[1] * dt_s) + (1.0f - k) * pitch_accel;
}

const float* ComplementaryFilter::get_integrated_gyro_rad()
{
    return m_integrated_gyro_rad;
}

const float* ComplementaryFilter::get_angular_speed_rad_s()
{
    return m_angular_speed_rad_s;
}

float ComplementaryFilter::get_roll_degree()
{
    return m_roll_rad * RAD_TO_DEG;
}

float ComplementaryFilter::get_pitch_degree()
{
    return m_pitch_rad * RAD_TO_DEG;
}

void ComplementaryFilter::get_roll_pitch(float* val)
{
    val[0] = m_roll_rad * RAD_TO_DEG;
    val[1] = m_pitch_rad * RAD_TO_DEG;
}