#include <imu/board-imu.hpp>
#include <task/task-imu.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Fetch a sample from the IMU.
 *
 * @return float
*/
float IMU::fetch_sample()
{
    return 0.0f;
}

/**
 * Fetch an acceleration sample from the IMU.
 *
 * @return float*
*/
float* IMU::fetch_accel()
{
    return task_imu_get_accel();
}

/**
 * Fetch an gyroscopic sample from the IMU.
 *
 * @return float*
*/
float* IMU::fetch_gyro()
{
    return task_imu_get_gyro();
}

/**
 * Fetch a roll pitch sample from the IMU.
 *
 * @return float*
*/
float* IMU::fetch_roll_pitch()
{
    return task_imu_get_roll_pitch();
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
