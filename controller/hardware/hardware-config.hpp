#pragma once

#include <cstdint>

static constexpr uint16_t IN1_NEAR_5V               = 4800;     //  [mV]  higher values mean that connection 1 is floating
static constexpr uint16_t IN1_NEAR_PIN2             = 3100;     //  [mV]  higher values mean that connection 1 is shorted to connection 2 (5000 * 18K / (18K + 10k))
static constexpr uint16_t IN1_TOUCH_HIGH            = 950;      //  [mV]  values in between these limits means that an old Touch Sensor is connected
static constexpr uint16_t IN1_TOUCH_LOW             = 850;      //  [mV]
static constexpr uint16_t IN1_NEAR_GND              = 100;      //  [mV]  lower  values mean that connection 1 is shorted to connection 3

static constexpr uint16_t OUT5_I2C_HIGH             = 3700;  //  [mV]  values in between these limits means that an old I2C Sensor or Colour Sensor is connected
static constexpr uint16_t OUT5_I2C_LOW              = 2800;  //  [mV]
static constexpr uint16_t OUT5_MINI_TACHO_HIGH1     = 2000;  //  [mV]  values in between these limits means that a mini tacho motor is pulling high when pin5 is pulling low
static constexpr uint16_t OUT5_MINI_TACHO_LOW1      = 1600;  //  [mV]
static constexpr uint16_t OUT5_BALANCE_HIGH         = 2600;  //  [mV]  values in between these limits means that connection 5 is floating
static constexpr uint16_t OUT5_BALANCE_LOW          = 2400;  //  [mV]
static constexpr uint16_t OUT5_LIGHT_HIGH           = 850;   //  [mV]  values in between these limits means that an old Light Sensor is connected
static constexpr uint16_t OUT5_LIGHT_LOW            = 650;   //  [mV]
static constexpr uint16_t OUT5_MINI_TACHO_HIGH2     = 450;   //  [mV]  values in between these limits means that a mini Tacho Motor is pulling low when pin5 floats
static constexpr uint16_t OUT5_MINI_TACHO_LOW2      = 250;   //  [mV]
static constexpr uint16_t OUT5_NEAR_GND             = 100;   //  [mV]  lower  values mean that connection 5 is shorted to ground

static constexpr uint16_t IN6_NEAR_GND              = 150;   //  [mV]  lower  values mean that connection 6 is floating
