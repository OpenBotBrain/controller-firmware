#include <assert.h>
#include <stm-hal/hal-dma.hpp>

struct DMAConfig
{
    DMA_TypeDef* controller;
    DMA_Channel_TypeDef* instance;
    uint32_t irq_flag;
    FinishDMACb cb;
    void* param;
    uint16_t num_bytes;
    uint8_t irqn;
    bool registered;
};

static constexpr uint8_t DMA1_TOTAL_CHANNELS = 7;
static constexpr uint8_t DMA2_TOTAL_CHANNELS = 5;

static DMAConfig s_dma1[] =
{
    // Channel 1
    {
        .controller = DMA1,
        .instance = DMA1_Channel1,
        .irq_flag = (DMA_ISR_TCIF1 | DMA_ISR_HTIF1),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel1_IRQn,
        .registered = false,
    },
    // Channel 2
    {
        .controller = DMA1,
        .instance = DMA1_Channel2,
        .irq_flag = (DMA_ISR_TCIF2 | DMA_ISR_HTIF2),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel2_IRQn,
        .registered = false,
    },
    // Channel 3
    {
        .controller = DMA1,
        .instance = DMA1_Channel3,
        .irq_flag = (DMA_ISR_TCIF3 | DMA_ISR_HTIF3),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel3_IRQn,
        .registered = 0,
    },
    // Channel 4
    {
        .controller = DMA1,
        .instance = DMA1_Channel4,
        .irq_flag = (DMA_ISR_TCIF4 | DMA_ISR_HTIF4),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel4_IRQn,
        .registered = false,
    },
    // Channel 5
    {
        .controller = DMA1,
        .instance = DMA1_Channel5,
        .irq_flag = (DMA_ISR_TCIF5 | DMA_ISR_HTIF5),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel5_IRQn,
        .registered = false,
    },
    // Channel 6
    {
        .controller = DMA1,
        .instance = DMA1_Channel6,
        .irq_flag = (DMA_ISR_TCIF6 | DMA_ISR_HTIF6),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel6_IRQn,
        .registered = false,
    },
    // Channel 7
    {
        .controller = DMA1,
        .instance = DMA1_Channel7,
        .irq_flag = (DMA_ISR_TCIF7 | DMA_ISR_HTIF7),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA1_Channel7_IRQn,
        .registered = false,
    }
};

static DMAConfig s_dma2[] =
{
    // Channel 1
    {
        .controller = DMA2,
        .instance = DMA2_Channel1,
        .irq_flag = (DMA_ISR_TCIF1 | DMA_ISR_HTIF1),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA2_Channel1_IRQn,
        .registered = false,
    },
    // Channel 2
    {
        .controller = DMA2,
        .instance = DMA2_Channel2,
        .irq_flag = (DMA_ISR_TCIF2 | DMA_ISR_HTIF2),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA2_Channel2_IRQn,
        .registered = false,
    },
    // Channel 3
    {
        .controller = DMA2,
        .instance = DMA2_Channel3,
        .irq_flag = (DMA_ISR_TCIF3 | DMA_ISR_HTIF3),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA2_Channel3_IRQn,
        .registered = false,
    },
    // Channel 4
    {
        .controller = DMA2,
        .instance = DMA2_Channel4,
        .irq_flag = (DMA_ISR_TCIF4 | DMA_ISR_HTIF4),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA2_Channel4_IRQn,
        .registered = false,
    },
    // Channel 5
    {
        .controller = DMA2,
        .instance = DMA2_Channel5,
        .irq_flag = (DMA_ISR_TCIF5 | DMA_ISR_HTIF5),
        .cb = nullptr,
        .param = nullptr,
        .num_bytes = 0,
        .irqn = DMA2_Channel5_IRQn,
        .registered = false,
    },
};

static DMAConfig* dma_typedef_to_info(DMAController controller, uint8_t channel)
{
    if (controller == DMAController::DMA1)
    {
        assert(channel < DMA1_TOTAL_CHANNELS);
        return &(s_dma1[channel]);
    }
    else if (controller == DMAController::DMA2)
    {
        assert(channel < DMA2_TOTAL_CHANNELS);
        return &(s_dma2[channel]);
    }

    assert(0);
    return nullptr;
}

static DMA_Channel_TypeDef* get_dma_intance(DMAController controller, uint8_t channel)
{
    if (controller == DMAController::DMA1)
    {
        assert(channel < DMA1_TOTAL_CHANNELS);
        return s_dma1[channel].instance;
    }
    else if (controller == DMAController::DMA2)
    {
        assert(channel < DMA2_TOTAL_CHANNELS);
        return s_dma2[channel].instance;
    }

    assert(0);
    return nullptr;
}

void dma_start_transaction(DMAController controller, uint8_t channel,
    uint32_t mem_addr, uint16_t num_bytes, uint32_t periph_addr,
    bool meminc, uint32_t dma_dir, bool circular_mode,
    uint32_t bits_per_transfer, bool enable_half_transfer,
    FinishDMACb cb, void* param)
{
    uint32_t periph_datasize;
    uint32_t memory_datasize;
    assert(bits_per_transfer == 8 || bits_per_transfer == 16 || bits_per_transfer == 32);

    DMAConfig* dma = dma_typedef_to_info(controller, channel);
    assert(dma->registered);

    DMA_HandleTypeDef hdma;
    s_hdma_adc1.Instance = get_dma_intance(controller, channel);

    if (bits_per_transfer == 8)
    {
        hdma.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    }
    else if (bits_per_transfer == 16)
    {
        hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    }
    else
    {
        hdma.Init.PeriphDataAlignment = DMA_MDATAALIGN_WORD;
        hdma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    }

    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.Request = DMA_REQUEST_0;      // TODO!!!
    hdma.Init.Direction = dma_dir ? DMA_PERIPH_TO_MEMORY : DMA_MEMORY_TO_PERIPH;
    hdma.Init.MemInc = meminc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
    hdma.Init.Mode = circular_mode ? DMA_CIRCULAR : DMA_NORMAL;
    hdma.Init.Priority = DMA_PRIORITY_HIGH;

    dma->cb = cb;
    dma->param = param;
    dma->num_bytes = num_bytes;
    if (dma->cb != nullptr)
    {
        if (enable_half_transfer)
        {
            __HAL_DMA_ENABLE_IT(hdma, DMA_IT_HT);
        }
        __HAL_DMA_ENABLE_IT(hdma, DMA_IT_TC);
    }
    dma->controller->IFCR = dma->irq_flag;

    HAL_ADC_Start_DMA(hdma, (uint32_t*)&mem_addr, num_bytes3);
}

static __attribute__((always_inline)) inline void dma_handler(DMAConfig* dmax, int total_channel)
{
    for (int i = 0; i < total_channel; i++)
    {
        DMAConfig* dma = &dmax[i];
        if ((dma->controller->ISR & dma->irq_flag))
        {
            dma->controller->IFCR = dma->irq_flag;   // clean flag
            if (dma->cb != nullptr)
            {
                dma->cb(dma->param);
            }
        }
    }
}

uint16_t dma_get_curr_data_counter(DMAType controller, uint8_t channel)
{
    return ((uint16_t)__HAL_DMA_GET_COUNTER(get_dma_intance(controller, channel)));
}

extern "C" {
void DMA1_Channel1_IRQHandler(void)
{
    dma_handler(&s_dma1[0], 1);
}

void DMA1_Channel2_IRQHandler(void)
{
    dma_handler(&s_dma1[1], 1);
}

void DMA1_Channel3_IRQHandler(void)
{
    dma_handler(&s_dma1[2], 1);
}

void DMA1_Channel4_IRQHandler(void)
{
    dma_handler(&s_dma1[3], 1);
}

void DMA1_Channel5_IRQHandler(void)
{
    dma_handler(&s_dma1[4], 1);
}

void DMA1_Channel6_IRQHandler(void)
{
    dma_handler(&s_dma1[5], 1);
}

void DMA1_Channel7_IRQHandler(void)
{
    dma_handler(&s_dma1[6], 1);
}

void DMA2_Channel1_IRQHandler(void)
{
    dma_handler(&s_dma2[0], 1);
}

void DMA2_Channel2_IRQHandler(void)
{
    dma_handler(&s_dma2[1], 1);
}

void DMA2_Channel3_IRQHandler(void)
{
    dma_handler(&s_dma2[2], 1);
}

void DMA2_Channel4_IRQHandler(void)
{
    dma_handler(&s_dma2[3], 1);
}

void DMA2_Channel5_IRQHandler(void)
{
    dma_handler(&s_dma2[4], 1);
}

}
