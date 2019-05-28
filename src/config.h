#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ShiftStepper.h"

//
#define SHIFT_REG_DATA 2
#define SHIFT_REG_CLOCK 4
#define SHIFT_REG_LATCH 5

#define SERVO           9

#define NFC_SS_PIN      10

#define SOFT_SERIAL_T   6
#define SOFT_SERIAL_R   7

//
#define STEPS_PER_TURN    2048.0f

#define CIRCUMFERENCE_MM  230.0f
#define WHEEL_DISTANCE    82.2f

#define STEPS_PER_MM      STEPS_PER_TURN / CIRCUMFERENCE_MM
#define STEPS_PER_DEGREE  ((WHEEL_DISTANCE * 3.1416) / 360) * STEPS_PER_MM

#define PENUP_DELAY 1100
#define PENDOWN_DELAY 2000

#endif
