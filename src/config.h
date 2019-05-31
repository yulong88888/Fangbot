#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ShiftStepper.h"

//电机
#define SHIFT_REG_DATA 14  //A0
#define SHIFT_REG_CLOCK 15 //A1
#define SHIFT_REG_LATCH 16 //A2

//蓝牙串口
#define SOFT_SERIAL_BLE_T 4
#define SOFT_SERIAL_BLE_R 5

//MP3串口
#define SOFT_SERIAL_MP3_T 6
#define SOFT_SERIAL_MP3_R 7

//舵机
#define SERVO 9

//NFC
#define NFC_SS_PIN 10

#define STEPS_PER_TURN 2048.0f

#define CIRCUMFERENCE_MM 230.0f
#define WHEEL_DISTANCE 82.2f

#define STEPS_PER_MM STEPS_PER_TURN / CIRCUMFERENCE_MM
#define STEPS_PER_DEGREE ((WHEEL_DISTANCE * 3.1416) / 360) * STEPS_PER_MM

#define PENUP_DELAY 1100
#define PENDOWN_DELAY 2000

enum CMD { GO = 0x31, BACK, LEFT, RIGHT, SERVO_UP, SERVO_DOWN, NFC_MP3 };

#endif
