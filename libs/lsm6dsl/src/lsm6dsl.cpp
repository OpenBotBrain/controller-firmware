#include <assert.h>
#include <lsm6dsl/lsm6dsl.hpp>
#include <lsm6dsl/lsm6dsl-reg.hpp>
#include <cstring>

LSM6DS::LSM6DS(const Config& config) : m_config(config)
{

}

bool LSM6DS::read(uint8_t reg, uint8_t& data)
{
    uint8_t read_data;
    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(true);
    }

    bool ret = m_config.read_cb(m_config.address, reg, &read_data, 1);
    if (ret)
    {
        data = read_data;
    }

    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(false);
    }
    return ret;
}

bool LSM6DS::write(uint8_t reg, uint8_t data)
{
    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(true);
    }
    bool ret = m_config.write_cb(m_config.address, reg, &data, 1);

    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(false);
    }
    return ret;
}

bool LSM6DS::read_multiple(uint8_t reg, uint8_t* data, int size)
{
    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(true);
    }
    bool ret = m_config.read_cb(m_config.address, reg, data, size);

    if (m_config.chip_select_cb)
    {
        m_config.chip_select_cb(false);
    }

    return ret;
}

bool LSM6DS::accelerometer_init()
{
    uint8_t ctrl = 0x00;
    uint8_t tmp;

    // Read CTRL1_XL
    if (!read(LSM6DSL_ACC_GYRO_CTRL1_XL, tmp))
    {
        return false;
    }

    // Write value to ACC MEMS CTRL1_XL register: FS and Data Rate
    ctrl = (uint8_t) LSM6DSL_ODR_208Hz;                         // TODO: IT SHOULD COME FROM CONFIG!
    tmp &= ~(0xFC);
    tmp |= ctrl;

    if (!write(LSM6DSL_ACC_GYRO_CTRL1_XL, tmp))
    {
        return false;
    }

    // Read CTRL3_C
    if (!read(LSM6DSL_ACC_GYRO_CTRL3_C, tmp))
    {
        return false;
    }

    // Write value to ACC MEMS CTRL3_C register: BDU and Auto-increment
    ctrl = (uint8_t) (LSM6DSL_BDU_CONTINUOS | LSM6DSL_ACC_GYRO_IF_INC_ENABLED); // TODO: IT SHOULD COME FROM CONFIG!
    tmp &= ~(0x44);
    tmp |= ctrl;

    if (!write(LSM6DSL_ACC_GYRO_CTRL3_C, tmp))
    {
        return false;
    }

    // TODO: CONFIGURE SENSITIVITY!!!!

    // Read the acceleration control register content
    uint8_t ctrlx;
    if (!read(LSM6DSL_ACC_GYRO_CTRL1_XL, ctrlx))
    {
        return false;
    }

    // Normal mode
    static constexpr float GRAVITY_VALUE = 9.8f;
    float max_reading = GRAVITY_VALUE / 32767.0f;

    switch (ctrlx & 0x0C)
    {
        case LSM6DSL_ACC_FULLSCALE_2G:
            m_acel_val_to_nm2 = max_reading * 2.0f;
            break;

        case LSM6DSL_ACC_FULLSCALE_4G:
            m_acel_val_to_nm2 = max_reading * 4.0f;
            break;

        case LSM6DSL_ACC_FULLSCALE_8G:
            m_acel_val_to_nm2 = max_reading * 8.0f;
            break;

        case LSM6DSL_ACC_FULLSCALE_16G:
            m_acel_val_to_nm2 = max_reading * 16.0f;
            break;
        default: break;
    }


    return true;
}

bool LSM6DS::gyro_init()
{
    uint8_t ctrl = 0x00;
    uint8_t tmp;

    // Read CTRL2_G
    if (!read(LSM6DSL_ACC_GYRO_CTRL2_G, tmp))
    {
        return false;
    }

    // Write value to GYRO MEMS CTRL2_G register: FS and Data Rate
    ctrl = (uint8_t) LSM6DSL_ODR_208Hz;
    tmp &= ~(0xFC);
    tmp |= ctrl;
    if (!write(LSM6DSL_ACC_GYRO_CTRL2_G, tmp))
    {
        return false;
    }

    // Read CTRL3_C
    if (!read(LSM6DSL_ACC_GYRO_CTRL3_C, tmp))
    {
        return false;
    }

    // Write value to GYRO MEMS CTRL3_C register: BDU and Auto-increment
    ctrl = (uint8_t) (LSM6DSL_BDU_CONTINUOS | LSM6DSL_ACC_GYRO_IF_INC_ENABLED); // TODO: IT SHOULD COME FROM CONFIG!
    tmp &= ~(0x44);
    tmp |= ctrl;
    if (!write(LSM6DSL_ACC_GYRO_CTRL3_C, tmp))
    {
        return false;
    }


    // Read the gyro control register content */
    uint8_t ctrlg;
    if (!read(LSM6DSL_ACC_GYRO_CTRL2_G, ctrlg))
    {
        return false;
    }

    // Normal mode
    static constexpr float MAX_READING = 32767.0f;
    switch(ctrlg & 0x0C)
    {
        case LSM6DSL_GYRO_FS_245:
            m_gyro_val_to_dps = 245 / MAX_READING;
            break;
        case LSM6DSL_GYRO_FS_500:
            m_gyro_val_to_dps = 500 / MAX_READING;
            break;
        case LSM6DSL_GYRO_FS_1000:
            m_gyro_val_to_dps = 1000 / MAX_READING;
            break;
        case LSM6DSL_GYRO_FS_2000:
            m_gyro_val_to_dps = 2000 / MAX_READING;
            break;
        default: break;
    }

    return true;
}

bool LSM6DS::check_imu_id()
{
    uint8_t tmp;

    // Read CTRL1_XL
    if (!read(LSM6DSL_ACC_GYRO_WHO_AM_I_REG, tmp))
    {
        return false;
    }

    return tmp == 0X6A;
}

bool LSM6DS::init()
{
    assert (m_config.read_cb != nullptr);
    assert (m_config.write_cb != nullptr);

    if (!check_imu_id())
    {
        return false;
    }

    m_init_success = accelerometer_init();
    m_init_success &= gyro_init();

    return m_init_success;
}

bool LSM6DS::read_accelerometer()
{
    int16_t pnRawData[TOTAL_AXIS];
    uint8_t buffer[TOTAL_AXIS * 2];

    // Read output register X, Y & Z acceleration
    if (!read_multiple(LSM6DSL_ACC_GYRO_OUTX_L_XL, buffer, 6))
    {
        return false;
    }

    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        pnRawData[i] = ((((uint16_t)buffer[2*i+1]) << 8) + (uint16_t)buffer[2*i]);
    }

    // Obtain the mg value for the three axis
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        m_acceleration[i] =(float)(pnRawData[i] * m_acel_val_to_nm2);
    }

    return true;
}

bool LSM6DS::read_gyro()
{
    int16_t pnRawData[TOTAL_AXIS];
    uint8_t buffer[TOTAL_AXIS * 2];

    // Read output register X, Y & Z acceleration
    if (!read_multiple(LSM6DSL_ACC_GYRO_OUTX_L_G, buffer, 6))
    {
        return false;
    }

    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        pnRawData[i] = ((((uint16_t)buffer[2*i+1]) << 8) + (uint16_t)buffer[2*i]);
    }

    // Obtain the mg value for the three axis
    for (int i = 0; i < TOTAL_AXIS; i++)
    {
        m_gyro[i] =(float)(pnRawData[i] * m_gyro_val_to_dps);
    }

    return true;
}

void LSM6DS::update()
{
    if (!m_init_success)
    {
        return;
    }

    read_accelerometer();
    read_gyro();
}

void LSM6DS::get_acceleration(float* output)
{
    std::memcpy(output, m_acceleration, sizeof(m_acceleration));
}

void LSM6DS::get_gyro(float* output)
{
    std::memcpy(output, m_gyro, sizeof(m_gyro));
}