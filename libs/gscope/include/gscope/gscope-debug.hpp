// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#ifdef __cplusplus

#include <gscope/gscope-types.hpp>

bool GSDebugColor(GSColor color, const char* p_string, ...);

extern "C" {
bool GSDebug(const char* p_string, ...);
}

#else

#include <stdbool.h>
bool GSDebug(const char* p_string, ...);
#endif