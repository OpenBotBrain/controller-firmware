#pragma once

#include <cstdint>

void hal_rng_start();

void hal_rng_stop();

uint32_t hal_rng_get();

uint32_t hal_rng_get_once();
