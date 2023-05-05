#pragma once

#include <stm32l4xx_hal.h>
#include <stm-hal/hal-board-init.hpp>

enum TimerType
{
    TIMER_TYPE_MOTOR_A_PWM = 0,
    TIMER_TYPE_MOTOR_B_PWM,
    TIMER_TYPE_MOTOR_C_PWM,
    TIMER_TYPE_MOTOR_D_PWM,
    TIMER_TYPE_MOTOR_A_ENCODER,
    TIMER_TYPE_MOTOR_B_ENCODER,
    TIMER_TYPE_MOTOR_C_ENCODER,
    TIMER_TYPE_MOTOR_D_ENCODER,
    TIMER_TYPE_PIEZO_PWM,

    TIMER_TYPE_TOTAL
};


void hal_tim_init_default(const BoardSpecificConfig* board_config);
void hal_tim_pwm_init(uint8_t type);
void hal_tim_pwm_set_pwm(uint8_t type, float duty);
void hal_tim_encoder_init(uint8_t type);
uint32_t hal_tim_encoder_get_tick(uint8_t type);

extern "C" {
uint32_t hal_timer_32_ms(void);
uint32_t hal_timer_32_us(void);
}