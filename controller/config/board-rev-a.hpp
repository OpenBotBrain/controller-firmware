#pragma once

#include <config/board-rev.hpp>
#include <stm-hal/hal-gpio.hpp>

const GPIOInitConfig* board_rev_a_get_gpio_config();
const BoardSpecificConfig* board_rev_a_get_specific_config();