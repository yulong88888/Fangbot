#include "nfc.h"
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>

MFRC522 *mfrc522;
SoftwareSerial *mp3Serial;

void NFC::setup(int SS_PIN, int receivePin, int transmitPin) {
  mp3Serial = new SoftwareSerial(receivePin, transmitPin);
  mp3Serial->begin(9600);

  mfrc522 = new MFRC522(SS_PIN, NULL);
  SPI.begin();  // Init SPI bus
  mfrc522->PCD_Init();
}

void NFC::handler() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  if (!mfrc522->PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522->PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid));

  Serial.print(F("Name: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  status = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key,
                                     &(mfrc522->uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522->GetStatusCodeName(status));
    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();
    return;
  }

  status = mfrc522->MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522->GetStatusCodeName(status));
    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();
    return;
  }

  // PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer1[i] != 32) {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");
  Serial.println(F("\n**End Reading**\n"));

  //数据头
  sendBuffer[0] = 0xAA;
  //数据命令
  sendBuffer[1] = 0x08;
  //数据长度
  byte len2 = getBytesLength(buffer1);
  sendBuffer[2] = len2 + 1;
  // flash U盘 sd卡 USB:00 SD:01 FLASH:02 NO_DEVICE：FF
  sendBuffer[3] = 0x02;
  //数据
  for (int i = 0; i < len2; i++) {
    sendBuffer[4 + i] = buffer1[i];
  }
  // SM
  int count = 0;
  for (int i = 0; i < 17; i++) {
    count += sendBuffer[i];
  }
  sendBuffer[17] = count & 0xff;
  Serial.println();
  dump_byte_array(sendBuffer, 18);

  delay(1);
  readSuccess = true;

  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
}

bool NFC::checkReady() { return readSuccess; }

void NFC::play() {
  mp3Serial->write((char *)sendBuffer);
  readSuccess = false;
}

int NFC::getBytesLength(byte *buffer) {
  int len = 0;
  for (byte i = 0; i < 16; i++) {
    if (buffer[i] != 0x00) {
      len++;
    }
  }
  return len;
}

void NFC::dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
