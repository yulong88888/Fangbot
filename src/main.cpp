#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include "QList.h"
#include "ble.h"
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
void callback(char cmd);
void doTask();

Servo penServo;

NFC mp3nfc;

BLE ble;

String comData = "";

QList<char> taskList;

void setup() {
  Serial.begin(115200);
  left = new ShiftStepper(0);
  right = new ShiftStepper(1);
  left->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
  right->setup(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);

  shiftStepperThread->setInterval(0.2);
  shiftStepperThread->onRun(shiftStepper);

  penServo.setup(SERVO, PENUP_DELAY, PENDOWN_DELAY);

  mp3nfc.setup(NFC_SS_PIN, SOFT_SERIAL_MP3_R, SOFT_SERIAL_MP3_T);

  ble.setup(SOFT_SERIAL_BLE_R, SOFT_SERIAL_BLE_T);

  servoThread->setInterval(0.2);
  servoThread->onRun(servo);

  mp3nfcThread->setInterval(1000);
  mp3nfcThread->onRun(nfc);

  // controll.add(shiftStepperThread);
  // controll.add(servoThread);
  // controll.add(mp3nfcThread);

  Serial.println("OK , Let's GO");
}

void loop() {
  //线程控制运行
  controll.run();
  //处理蓝牙信息
  ble.handlerMsg(callback);
  //蓝牙配置
  if (Serial.available()) {
    char temp = char(Serial.read());
    comData += temp;
    if (temp == '\n') {
      Serial.flush();
      Serial.print(comData);
      ble.config(comData);
      comData = "";
    }
  }
}

void doTask() {
  char task = taskList.at(0);
  Serial.println("dotask");
  switch (task) {
    case GO:
      Serial.println("GO");
      controll.add(shiftStepperThread);
      left->turn(100 * steps_per_mm, FORWARD);
      right->turn(100 * steps_per_mm, BACKWARD);
      break;
    case BACK:
      Serial.println("BACK");
      controll.add(shiftStepperThread);
      left->turn(100 * steps_per_mm, BACKWARD);
      right->turn(100 * steps_per_mm, FORWARD);
      break;
    case LEFT:
      Serial.println("LEFT");
      controll.add(shiftStepperThread);
      left->turn(100 * steps_per_mm, FORWARD);
      right->turn(100 * steps_per_mm, FORWARD);
      break;
    case RIGHT:
      Serial.println("RIGHT");
      controll.add(shiftStepperThread);
      left->turn(100 * steps_per_mm, BACKWARD);
      right->turn(100 * steps_per_mm, BACKWARD);
      break;
    case SERVO_UP:
      controll.add(servoThread);
      Serial.println("SERVO_UP");
      penServo.setPenUp();
      break;
    case SERVO_DOWN:
      controll.add(servoThread);
      Serial.println("SERVO_DOWN");
      penServo.setPenDown();
      break;
    case NFC_MP3:
      Serial.println("NFC_MP3");
      mp3nfc.play();
      break;
  }
  taskList.pop_front();
}

/**
 * 蓝牙收到数据回调函数，将任务放到任务列表
 */
void callback(char cmd) {
  Serial.println("new task");
  Serial.println(cmd);
  taskList.push_back(cmd);
  doTask();
}

void shiftStepper() {
  if (left->ready() && right->ready()) {
    controll.remove(shiftStepperThread);
    return;
  }
  left->trigger();
  right->trigger();
}

// boolean flag = false;

void servo() {
  if (!penServo.ready()) {
    penServo.servoHandler();
  } else {
    controll.remove(servoThread);
  }

  // if (!penServo.ready()) {
  //   // Serial.print(".");
  //   penServo.servoHandler();
  // } else {
  //   // // Serial.print(",");
  //   // if (flag) {

  //   // } else {
  //   //   penServo.setPenDown();
  //   // }
  //   // flag = !flag;
  // }
}

void nfc() { mp3nfc.handler(); }
