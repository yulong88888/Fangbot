#ifndef __BLE_H__
#define __BLE_H__

#include "Arduino.h"

typedef void (* cb) (char cmd);

class BLE {
 public:
  void setup(int receivePin, int transmitPin);
  void handlerMsg(cb callback);
  void config(String data);
  void send(char cmd,bool isRevOrOK);

 private:
  String bleComData = "";
  bool wait(String data);
  char doMsg(String str);
  void split(char *src, const char *separator, char **dest);
};

#endif
