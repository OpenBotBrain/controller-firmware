#pragma once

#include <cstdint>

enum class RNG_Status
{
    ENABLED,
    DISABLED
};

void hal_rng_start();

void hal_rng_stop();

uint32_t hal_rng_get();

uint32_t hal_rng_get_once();

RNG_Status hal_rng_get_status();
