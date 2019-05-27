#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Servo.h>
#include "config.h"

ShiftStepper *left;
ShiftStepper *right;

Servo myservo;

ThreadController controll = ThreadController();
Thread *shiftStepperThread = new Thread();
Thread *servoThread = new Thread();

float steps_per_mm = STEPS_PER_MM;
float steps_per_degree = STEPS_PER_DEGREE;
int penup_delay = PENUP_DELAY;
int pendown_delay = PENDOWN_DELAY;
int wheel_distance = WHEEL_DISTANCE;

void shiftStepper();
void servo();

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

  myservo.attach(SERVO); 

  servoThread->setInterval(3000);
  servoThread->onRun(servo);
  
  // controll.add(shiftStepperThread);
  controll.add(servoThread);
}

void loop() { controll.run(); }

void shiftStepper() {
  if (left->ready() && right->ready()) {
    return;
  }
  left->trigger();
  right->trigger();
}

void servo() {
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
}
