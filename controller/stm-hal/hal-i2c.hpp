#pragma once

#include <cstdint>

enum I2CTypes
{
    I2C_TYPE_PORT_INPUT_1 = 0,
    I2C_TYPE_PORT_INPUT_2,
    I2C_TYPE_PORT_INPUT_3,
    I2C_TYPE_SYSTEM,

    I2C_TYPE_TOTAL
};

using FinishI2CCb = void(*)(bool, void*, uint8_t*, uint16_t);  // error, param, data, size

void hal_i2c_init_default(uint8_t board_rev);
void hal_i2c_init(uint8_t type);
bool hal_i2c_read(uint8_t type, uint8_t address, uint8_t reg,
    uint16_t data_size, FinishI2CCb finish_tx_cb, void* param);
bool hal_i2c_write(uint8_t type, uint8_t address, uint8_t reg,
    uint8_t* buf, uint16_t buf_size, FinishI2CCb finish_tx_cb, void* param);