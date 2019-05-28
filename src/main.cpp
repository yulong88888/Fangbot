#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include "config.h"
#include "nfc.h"
#include "servo.h"

ShiftStepper *left;
ShiftStepper *right;

ThreadController controll = ThreadController();
Thread *shiftStepperThread = new Thread();
Thread *servoThread = new Thread();
Thread *mp3nfcThread = new Thread();

float steps_per_mm = STEPS_PER_MM;
float steps_per_degree = STEPS_PER_DEGREE;
int wheel_distance = WHEEL_DISTANCE;

void shiftStepper();
void servo();
void nfc();

Servo penServo;

NFC mp3nfc;

void setup() {
  Serial.begin(115200);
  left = new ShiftStepper(0);
  right = new ShiftStepper(1);
  left->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
  right->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);

  left->turn(100 * steps_per_mm, BACKWARD);
  right->turn(100 * steps_per_mm, FORWARD);

  shiftStepperThread->setInterval(0.10);
  shiftStepperThread->onRun(shiftStepper);

  penServo.setup(SERVO, PENUP_DELAY, PENDOWN_DELAY);

  mp3nfc.setup(NFC_SS_PIN, SOFT_SERIAL_R, SOFT_SERIAL_T);

  servoThread->setInterval(1);
  servoThread->onRun(servo);

  mp3nfcThread->setInterval(5);
  mp3nfcThread->onRun(nfc);

  // controll.add(shiftStepperThread);
  // controll.add(servoThread);
  controll.add(mp3nfcThread);

  // penServo.setPenUp();
}

void loop() { controll.run(); }

void shiftStepper() {
  if (left->ready() && right->ready()) {
    return;
  }
  left->trigger();
  right->trigger();
}

boolean flag = false;

void servo() {
  if (!penServo.ready()) {
    // Serial.print(".");
    penServo.servoHandler();
  } else {
    // // Serial.print(",");
    // if (flag) {

    // } else {
    //   penServo.setPenDown();
    // }
    // flag = !flag;
  }
}

void nfc() { mp3nfc.handler(); }
