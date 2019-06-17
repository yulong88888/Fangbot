#ifndef __NFC_H__
#define __NFC_H__

#include "Arduino.h"

class NFC {
 public:
  void setup(int SS_PIN, int JQ8900_BUSY, int receivePin, int transmitPin);
  void handler();
  void play();
  bool checkReady();

 private:
  int getBytesLength(byte *buffer);
  void dump_byte_array(byte *buffer, byte bufferSize);
  byte sendBuffer[18];
  bool readSuccess = false;
  int PIN_JQ8900_BUSY = 0;
};

#endif
