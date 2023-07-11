#pragma once

#define   IN1_NEAR_5V                   4800  //  [mV]  higher values mean that connection 1 is floating
#define   IN1_NEAR_PIN2                 3100  //  [mV]  higher values mean that connection 1 is shorted to connection 2 (5000 * 18K / (18K + 10k))
#define   IN1_TOUCH_HIGH                950   //  [mV]  values in between these limits means that an old Touch Sensor is connected
#define   IN1_TOUCH_LOW                 850   //  [mV]
#define   IN1_NEAR_GND                  100   //  [mV]  lower  values mean that connection 1 is shorted to connection 3

#define   OUT5_I2C_HIGH                 3700  //  [mV]  values in between these limits means that an old I2C Sensor or Colour Sensor is connected
#define   OUT5_I2C_LOW                  2800  //  [mV]
#define   OUT5_MINI_TACHO_HIGH1         2000  //  [mV]  values in between these limits means that a mini tacho motor is pulling high when pin5 is pulling low
#define   OUT5_MINI_TACHO_LOW1          1600  //  [mV]
#define   OUT5_BALANCE_HIGH             2600  //  [mV]  values in between these limits means that connection 5 is floating
#define   OUT5_BALANCE_LOW              2400  //  [mV]
#define   OUT5_LIGHT_HIGH               850   //  [mV]  values in between these limits means that an old Light Sensor is connected
#define   OUT5_LIGHT_LOW                650   //  [mV]
#define   OUT5_MINI_TACHO_HIGH2         450   //  [mV]  values in between these limits means that a mini Tacho Motor is pulling low when pin5 floats
#define   OUT5_MINI_TACHO_LOW2          250   //  [mV]
#define   OUT5_NEAR_GND                 100   //  [mV]  lower  values mean that connection 5 is shorted to ground

#define   IN6_NEAR_GND                  150   //  [mV]  lower  values mean that connection 6 is floating
