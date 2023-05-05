#pragma once

#include <stm-hal/hal-datatype.hpp>

void hal_board_init(const BoardSpecificConfig* board_config);
const BoardSpecificConfig* board_get_specific_configuration();
