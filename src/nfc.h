#ifndef __NFC_H__
#define __NFC_H__

#include "Arduino.h"

class NFC {
 public:
  void setup(int SS_PIN, int receivePin, int transmitPin);
  void handler();
  void play();

 private:
  int getBytesLength(byte *buffer);
  void dump_byte_array(byte *buffer, byte bufferSize);
  byte sendBuffer[18];
};

#endif
