#pragma once

void system_imu_init(void);

void system_imu_update(void);

float* system_imu_accel();

float* system_imu_gyro();

float* system_imu_roll_pitch();
