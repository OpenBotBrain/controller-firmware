#pragma once

#include <cstdint>

class ComplementaryFilter
{
    public:
        static constexpr uint8_t TOTAL_AXIS = 3;
        static constexpr float PI = 3.141592653589793238f;
        static constexpr float DEG_TO_RAD = PI / 180.0f;
        static constexpr float RAD_TO_DEG = 180.0F / PI;

        static constexpr float FILTER_ACCEL_RELIABLE_POLE = 0.97f;
        static constexpr float FILTER_ACCEL_NON_RELIABLE_POLE = 0.995f;

        ComplementaryFilter();

        void init();
        void update(float* accel_m2s, float* gyro_dps, float dt_s);
        const float* get_integrated_gyro_rad();
        const float* get_angular_speed_rad_s();
        float get_roll_degree();
        float get_pitch_degree();
        void get_roll_pitch(float* val);

    private:
        float m_integrated_gyro_rad[TOTAL_AXIS];
        float m_gravity_magnitude_m2s {9.8};
        float m_omega_magnitude {0};
        float m_angular_speed_rad_s[TOTAL_AXIS];
        float m_roll_rad;
        float m_pitch_rad;
        int m_counter {0};

        void integrate_value_update(float* gyro_dps, float period_s);
        void gravity_magnitude_update(float new_val);
        void angular_velocity_update(float* gyro_dps);
        float get_filter_pole(float accel_magnitude);
        float get_module(float* data);
};