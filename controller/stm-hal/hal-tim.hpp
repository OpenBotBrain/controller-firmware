#pragma once

#include <cstdint>

enum TimerType
{
    TIMER_TYPE_NONE = 0,

    TIMER_TYPE_TOTAL
};

void hal_tim_init_default(uint8_t board_rev);
void hal_tim_init(uint8_t type);