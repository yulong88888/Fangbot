#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ShiftStepper.h"

//
#define SHIFT_REG_DATA 1
#define SHIFT_REG_CLOCK 2
#define SHIFT_REG_LATCH 3

#define SOFT_SERIAL_BLE_T   4
#define SOFT_SERIAL_BLE_R   5

#define SOFT_SERIAL_MP3_T   6
#define SOFT_SERIAL_MP3_R   7

#define SERVO           9

#define NFC_SS_PIN      10

//
#define STEPS_PER_TURN    2048.0f

#define CIRCUMFERENCE_MM  230.0f
#define WHEEL_DISTANCE    82.2f

#define STEPS_PER_MM      STEPS_PER_TURN / CIRCUMFERENCE_MM
#define STEPS_PER_DEGREE  ((WHEEL_DISTANCE * 3.1416) / 360) * STEPS_PER_MM

#define PENUP_DELAY 1100
#define PENDOWN_DELAY 2000

#endif
