#pragma once

#include <cstdint>

enum SPIChannelType : uint8_t
{
    SPI_TYPE_IMU_FLASH = 0,

    SPI_TOTAL
};

using FinishSPICb = void(*)(void*);

void hal_spi_init(uint8_t type, FinishSPICb cb, void* param);