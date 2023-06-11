#pragma once

namespace ModuleCommon
{
    // 4.7k and 2.4k -> 0.6619718(calc) or 0.6633819 (Measured)
    static constexpr float ADC_TO_VOLTAGE_INPUTS = 0.0012347766;//0.00127364196f;

    static constexpr float LOW_PASS_FILTER_POLE_Z = 0.8;
    static constexpr void low_pass_filter_update(float& acum, float new_val)
    {
        acum = acum * LOW_PASS_FILTER_POLE_Z + new_val * (1.0f - LOW_PASS_FILTER_POLE_Z);
    }
}