#include <imu/board-imu.hpp>
#include <task/task-imu.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

/**
 * Inits the member IMU data.
*/
void IMU::init()
{
    m_imu_data = IMU_Data
    {
        .accel = { 0.0f, 0.0f, 0.0f },
        .gyro  = { 0.0f, 0.0f, 0.0f },
        .roll  = { 0.0f, 0.0f }
    };
}

/**
 * Updates the IMU data.
*/
void IMU::update()
{
    m_imu_data = task_get_imu_data();
}

/**
 * Gets the IMU data.
 *
 * @return IMU_data
*/
IMU_Data IMU::get_imu_data()
{
    return m_imu_data;
}

// ------------------------------------------------------------------------------------
//                                      PRIVATE API
// ------------------------------------------------------------------------------------
