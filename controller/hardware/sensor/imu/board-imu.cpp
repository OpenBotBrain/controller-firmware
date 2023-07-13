#include <imu/board-imu.hpp>
//#include <system/system-imu.hpp>
#include <task/task-imu.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Init the IMU.
*/
void IMU::init()
{
    //system_imu_init();
}

/**
 * Update the IMU.
*/
void IMU::update()
{
    //system_imu_update();
}

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
