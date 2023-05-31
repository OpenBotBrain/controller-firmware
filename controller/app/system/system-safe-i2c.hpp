#pragma once

void system_safe_i2c_init();
bool system_safe_i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint8_t buf_size);
bool system_safe_i2c_read(uint8_t address, uint8_t reg, uint8_t* buf, uint8_t buf_size);