#pragma once

#include <cstdint>

class LSM6DS
{
    public:
        using RW = bool(*)(uint8_t dir, uint8_t reg, uint8_t* buf, uint16_t size);
        using CScb = void(*)(bool);
        using TimestampCb = uint32_t(*)();

        static constexpr uint8_t TOTAL_AXIS = 3;
        static constexpr uint8_t LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW = 0xD4;    // SAD[0] = 0
        static constexpr uint8_t  LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH = 0xD6;  // SAD[0] = 1

        struct Config
        {
            RW read_cb;
            RW write_cb;
            CScb chip_select_cb;
            TimestampCb get_timestamp_ms32_cb;
            uint8_t address;
        };

        LSM6DS(const Config& config);

        bool init();
        void update();
        void get_acceleration(float* buf);
        void get_gyro(float* buf);

    private:
        const Config& m_config;
        float m_acceleration[TOTAL_AXIS];
        float m_gyro[TOTAL_AXIS];
        bool m_init_success {false};
        float m_acel_val_to_nm2 {0.0};  // newton per square meter
        float m_gyro_val_to_dps {0.0};  // degree per seconds


        bool read(uint8_t reg, uint8_t& data);
        bool write(uint8_t reg, uint8_t data);
        bool read_multiple(uint8_t reg, uint8_t* data, int size);

        bool accelerometer_init();
        bool gyro_init();
        bool check_imu_id();
        bool read_accelerometer();
        bool read_gyro();

};