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
    boolean flag = wait("JDY-10M-V2.3-MESH");
    if (!flag) {
      Serial.println("0.0");
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

boolean BLE::wait(String data) {
  String comData = "";
  long time = millis();
  while (millis() - time < 1000) {
    if (bleSerial->available() > 0) {
      char temp = char(bleSerial->read());
      comData += temp;
      if (temp == '\n') {
        Serial.println(comData.c_str());
        if (data.indexOf(comData)) {
          Serial.println("fuck");
          bleSerial->flush();
          return true;
        }
      }
    }
  }
  return false;
}

void BLE::handlerMsg() {
  bleSerial->listen();
  while (bleSerial->available()) {
    char temp = char(bleSerial->read());
    bleComData += temp;
    if (temp == '\n') {
      Serial.print(bleComData);
      bleSerial->flush();
      bleComData = "";
    }
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
