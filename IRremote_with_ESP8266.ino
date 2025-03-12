/*
 This file demonstrates sending IR codes with sendRaw
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Protocol=NEC Address=0x4 Command=0x18 Raw-Data=0xE718FB04 32 bits LSB first
 *
 * If it is a supported protocol, it is more efficient to use the protocol send function
 * (here sendNEC) to send the signal.
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 */

#include <Arduino.h>

//This two line must be defined before #include <IRremote.hpp>
#define IR_RECEIVE_PIN 14  // GPIO14 (D5 on NodeMCU)
#define IR_SEND_PIN 12     // GPIO12 (D6 on NodeMCU)
#include <IRremote.hpp> //

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  IrSender.begin();  //WARNING: USE THE CORRECT GPIO PIN.
}

const uint16_t rawData[] = { 9180, 4470, 580, 570, 530, 570, 580, 520, 630, 520, 630, 520, 580, 520, 630, 570, 580, 520, 580, 1620, 630, 1620, 630, 1620, 580, 1620, 630, 520, 630, 1620, 580, 1620, 630, 1670, 530, 570, 580, 520, 630, 1620, 630, 520, 580, 520, 630, 570, 530, 570, 580, 520, 630, 1670, 530, 1670, 580, 570, 580, 1620, 580, 1620, 630, 1620, 630, 1620, 580, 1670, 580 };

void loop() {
  IrSender.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), NEC_KHZ);
  delay(1000);
}