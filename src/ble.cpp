#include "ble.h"
#include <SoftwareSerial.h>

SoftwareSerial *bleSerial;

void BLE::setup(int receivePin, int transmitPin) {
  bleSerial = new SoftwareSerial(receivePin, transmitPin);
  bleSerial->begin(9600);
}

void BLE::config(String data) {
  if ((data.indexOf("AT+NAME") == 0) && data.indexOf("AT+NETID") > 0 &&
      data.indexOf("AT+MADDR") > 0) {
    //先以9600通信，看结果
    bleSerial->begin(9600);
    //检测是否可以通信，如果可以通信就是配置过了
    bleSerial->println("AT+VER");
    bool flag = wait("JDY-10M-V2.3-MESH");
    if (!flag) {
      Serial.println("BLE:CONFIG 0.0");
      bleSerial->begin(115200);
      //改变波特率
      bleSerial->println("AT+BAUD4");
      wait("+OK");
      bleSerial->println("AT+RESET");
      wait("+OK");
      config(data);
      return;
    }

    char *temp[3] = {0};
    split((char *)data.c_str(), ",", temp);

    Serial.println("BLE:CONFIG");
    Serial.println(temp[0]);
    Serial.println(temp[1]);
    Serial.println(temp[2]);

    bleSerial->println(temp[0]);
    wait("+OK");
    bleSerial->println(temp[1]);
    wait("+OK");
    bleSerial->println(temp[2]);
    wait("+OK");

    bleSerial->println("AT+RESET");
    delay(1000);

    Serial.println("OK");
  } else {
    Serial.println("err");
  }
}

bool BLE::wait(String data) {
  String comData = "";
  long time = millis();
  while (millis() - time < 1000) {
    if (bleSerial->available() > 0) {
      char temp = char(bleSerial->read());
      comData += temp;
      if (temp == '\n') {
        Serial.println("BLE:WAIT");
        Serial.println(comData);
        if (data.indexOf(comData)) {
          Serial.println("fuck");
          return true;
        }
      }
    }
  }
  return false;
}

void BLE::handlerMsg(cb callback) {
  // bleSerial->listen();
  while (bleSerial->available()) {
    char temp = char(bleSerial->read());
    bleComData += temp;
    if (temp == '\n') {
      Serial.flush();
      Serial.println("BLE:handlerMsg");
      Serial.print(bleComData);
      char cmd = doMsg(bleComData);
      if (cmd != 0x00) {
        delay(100);
        Serial.println("cb...");
        callback(cmd);
        send(cmd, false);
      }
      bleComData = "";
    }
  }
}

char BLE::doMsg(String str) {
  if ((byte)str.charAt(0) == 0xAA && (byte)str.charAt(1) == 0x01 &&
      (byte)str.charAt(2) == 0x04 && (byte)str.charAt(4) == 0x0F &&
      (byte)str.charAt(5) == 0x0D && (byte)str.charAt(6) == 0x0A) {
    return str.charAt(3);
  } else {
    return 0x00;
  }
}

/**
 * 发送响应，（false）收到返回，（true）任务处理完
 */
void BLE::send(char cmd, bool isRevOrOK) {
  byte sendBuffer[8];
  sendBuffer[0] = 0xAA;
  sendBuffer[1] = 0xFB;
  sendBuffer[2] = 0x00;
  sendBuffer[3] = 0x01;
  sendBuffer[4] = cmd;
  if (isRevOrOK) {
    sendBuffer[5] = 0x02;
  } else {
    sendBuffer[5] = 0x01;
  }
  sendBuffer[6] = 0x0D;
  sendBuffer[7] = 0x0A;
  for (byte sb : sendBuffer) {
    // Serial.write(sb);
    bleSerial->write(sb);
  }
}

/**
 * 字符串分割
 */
void BLE::split(char *src, const char *separator, char **dest) {
  char *pNext;
  int count = 0;
  if (src == NULL || strlen(src) == 0) return;
  if (separator == NULL || strlen(separator) == 0) return;
  pNext = strtok(src, separator);
  while (pNext != NULL) {
    *dest++ = pNext;
    ++count;
    pNext = strtok(NULL, separator);
  }
}
