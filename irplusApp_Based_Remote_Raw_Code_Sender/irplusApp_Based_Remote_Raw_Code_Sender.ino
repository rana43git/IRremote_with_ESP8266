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

/*
 Some lines have been commented as paragraph. Thoses lines don't require to send signal succesfully instead those were for debugging through serial monitor. Commenting those lines also reduced time complexity.
 */

#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Include html files as header files
#include "index.h"

//This two line must be defined before #include <IRremote.hpp>
#define IR_RECEIVE_PIN 14  // GPIO14 (D5 on NodeMCU)
#define IR_SEND_PIN 12     // GPIO12 (D6 on NodeMCU)
#include <IRremote.hpp>    //

ESP8266WebServer server(80);

void sendIRRaw(String irData);

void handleIRCommand() {
  if (!server.hasArg("carrier") || !server.hasArg("code")) {
    server.send(400, "text/plain", "Missing carrier or code parameter");
    return;
  }

  else {
    // Extract carrier frequency and IR code
    int carrierHz = server.arg("carrier").toInt();  //Example: 38000
    int carrierKhz = (carrierHz / 1000);            //Example: 38
    String irData = server.arg("code");
    sendIRRaw(irData, carrierKhz);
  }
  server.send(200, "text/plain", "IR Signal Sent");
}

void routerSetUp() {
  server.close();
  Serial.println("");
  Serial.println("HTTP Server Stopped!");
  WiFiManager wmOnDemand;
  wmOnDemand.setConfigPortalTimeout(120);
  //wmOnDemand.startConfigPortal(); // auto generated AP name from chipid
  wmOnDemand.startConfigPortal("ESP_E153C6", "admin1234");  // password protected ap
  /*Although startConfigPortal() started devide is still connected to the router if wifi availavble.*/
  //wmOnDemand.resetSettings(); //wipe stored credentials
  Serial.println("<<Restarting the ESP>>");
  ESP.restart();  //remove this line and check if every thing still works.
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED and output pin(D6) off by making the voltage, necessarry else next signal might not send correctly

  IrSender.begin();  //WARNING: USE THE CORRECT GPIO PIN.

  WiFiManager wm;
  wm.autoConnect("ESP_E153C6", "admin1234");  // password protected ap

  server.on("/", []() {
    server.send_P(200, "text/html", INDEX);
  });
  server.on("/send", handleIRCommand);
  server.on("/router_setup", routerSetUp);
  server.begin();
  Serial.println("Web server started!");

  if (MDNS.begin("esp8266")) {
    Serial.println("\nMDNS responder started at http://esp8266.local");
  }
}

void loop() {
  MDNS.update();
  server.handleClient();
}

void sendIRRaw(String irData, int carrierKhz) {

  //Serial.println("Received IR Data: " + irData);

  uint16_t irValues[100];  // Use uint16_t instead of int
  int index = 0;
  char* token = strtok((char*)irData.c_str(), " ");  //The " " means irData is white space seperated

  while (token != NULL && index < 100) {
    irValues[index++] = (uint16_t)atoi(token);  // Convert to uint16_t
    token = strtok(NULL, " ");                  //The " " means irData is white space seperated
  }

  /*
  // Print the parsed IR values
  Serial.print("Parsed IR Values: ");
  for (int i = 0; i < index; i++) {
    Serial.print(irValues[i]);
    if (i < index - 1) {  //Only print comma if NOT the last value
      Serial.print(", ");
    }
  }
  Serial.println();  // New line after printing all values
*/
  IrSender.sendRaw(irValues, index, carrierKhz);  //Sending the IR Signal
  //Serial.println("IR Signal Sent!");
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED and output pin(D6) off by making the voltage, necessarry else next signal might not send correctly
}