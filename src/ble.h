#ifndef __BLE_H__
#define __BLE_H__

#include "Arduino.h"

class BLE {
 public:
  void setup(int receivePin, int transmitPin);
  void handlerMsg();
  void config(String data);

 private:
  String bleComData = "";
  boolean wait(String data);
  void split(char *src, const char *separator, char **dest);
};

#endif
