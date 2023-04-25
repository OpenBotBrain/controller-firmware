#pragma once

#include <cstdint>

enum I2CTypes
{
    I2C_TYPE_PORT_INPUT_1 = 0,
    I2C_TYPE_PORT_INPUT_2,
    I2C_TYPE_SYSTEM,
    I2C_TYPE_TOTAL
};

void hal_i2c_init_default(uint8_t board_rev);
void hal_i2c_init(uint8_t type);