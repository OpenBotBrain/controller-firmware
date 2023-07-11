#pragma once

#include <cstdint>

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

using TimerUpdateCb = void(*)(void*);

void hal_tim_pwm_init(uint8_t type);
void hal_tim_pwm_set_pwm(uint8_t type, float duty);
void hal_tim_encoder_init(uint8_t type);
uint32_t hal_tim_encoder_get_tick(uint8_t type);

uint32_t hal_tim_neoled_init(TimerUpdateCb cb, void* param);
void hal_timer_neoled_start_dma_transfer(uint8_t* data, uint32_t size);

uint32_t hal_timer_32_ms(void);
uint32_t hal_timer_32_us(void);
