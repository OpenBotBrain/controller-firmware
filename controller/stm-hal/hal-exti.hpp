#pragma once

#include <cstdint>

enum class TriggerType : uint8_t
{
    RISSING = 0,
    FALLING,
    RISSING_FALLING,
};

using FinishEXTICb = void(*)(void*);

void hal_exti_init(uint16_t gpio_id, TriggerType trigger, FinishEXTICb cb, void* param);
