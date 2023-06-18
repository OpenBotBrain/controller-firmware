#pragma once

#include <cstdint>

enum SPIChannelType : uint8_t
{
    SPI_TYPE_IMU_FLASH = 0,

    SPI_TOTAL
};

using FinishSPICb = void(*)(void*);

void hal_spi_init(uint8_t type, FinishSPICb cb, void* param);
bool hal_spi_transmit_receive(uint8_t type, uint8_t* tx_buff, uint8_t* rx_buff, uint32_t size);