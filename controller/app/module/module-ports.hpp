#pragma once

#include <module/module-input.hpp>
#include <module/module-output.hpp>

static constexpr int TOTAL_INPUT_PORTS = 4;
static constexpr int TOTAL_OUTPUT_PORTS = 4;

InputPort& module_port_get_input_port(uint8_t input_id);
OutputPort& module_port_get_outpur_port(uint8_t output_id);
void module_port_init();
void module_port_update();