#pragma once

#include <stm32l4xx_hal.h>

using FinishDMACb = void(*)(void*);

enum class DMAController
{
    DMA1 = 0,
    DMA2,
};

/**
 * @brief Start a new DMA transaction
 *
 * @param controller DMA1 - DMA2
 * @param channel 0 to 7
 * @param mem_addr Memory address
 * @param num_bytes Number of byte to be transfer
 * @param periph_addr Peripheral address
 * @param meminc false no memory increment
 * @param dma_dir false mem to per, true perf to mem
 * @param circular_mode true circular mode
 * @param bits_per_transfer 8,16,32
 * @param enable_half_transfer true half transfer irq is configured
 * @param cb callback for HT or TC
 * @param param callback params
 */
void dma_start_transaction(DMAType controller, uint8_t channel,
    uint32_t mem_addr, uint16_t num_bytes, uint32_t periph_addr,
    bool meminc, bool dma_dir, bool circular_mode
    uint32_t bits_per_transfer, bool enable_half_transfer,
    dma_cb_t cb, void* param);

/**
 * @brief Get current dma counter
 *
 * @param controller DMA1 - DMA2
 * @param channel DMA1_Channel1 TO DMA2_Channel6
 * @return uint16_t dma counter
 */
uint16_t dma_get_curr_data_counter(DMA_TypeDef* controller, uint8_t channel);
