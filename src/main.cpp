#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include "config.h"

ShiftStepper *left;
ShiftStepper *right;

ThreadController controll = ThreadController();
Thread *shiftStepperThread = new Thread();

float steps_per_mm = STEPS_PER_MM;
float steps_per_degree = STEPS_PER_DEGREE;
int penup_delay = PENUP_DELAY;
int pendown_delay = PENDOWN_DELAY;
int wheel_distance = WHEEL_DISTANCE;

void shiftStepper();

void setup() {
  Serial.begin(115200);
  // ShiftStepper::setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
  left = new ShiftStepper(0);
  right = new ShiftStepper(1);
  left->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
  right->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);

  left->turn(100 * steps_per_mm, BACKWARD);
  right->turn(100 * steps_per_mm, FORWARD);

  shiftStepperThread->setInterval(0.10);
  shiftStepperThread->onRun(shiftStepper);

  controll.add(shiftStepperThread);
}

void loop() { controll.run(); }

void shiftStepper() {
  if (left->ready() && right->ready()) {
    return;
  }
  Serial.print(".");
  left->trigger();
  right->trigger();
}
