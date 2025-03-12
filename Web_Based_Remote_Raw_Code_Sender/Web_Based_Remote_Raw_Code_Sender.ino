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
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//This two line must be defined before #include <IRremote.hpp>
#define IR_RECEIVE_PIN 14  // GPIO14 (D5 on NodeMCU)
#define IR_SEND_PIN 12     // GPIO12 (D6 on NodeMCU)
#include <IRremote.hpp>    //

const char* ssid = "ESP32-IR";  // Change this if needed
const char* password = "12345678";

ESP8266WebServer server(8080);

void sendIRRaw(String irData);

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage

  IrSender.begin();  //WARNING: USE THE CORRECT GPIO PIN.

  WiFi.softAP(ssid, password);
  Serial.println("WiFi Started!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", R"rawliteral(
      <html>
      <head>
        <title>ESP32 IR Remote</title>
        <script>
          function sendIR() {
            var irData = document.getElementById("irCode").value;
            fetch("/send?data=" + encodeURIComponent(irData))
              .then(response => response.text())
              .then(data => alert(data));
          }
        </script>
      </head>
      <body>
        <h2>ESP32 IR Remote</h2>
        <input type="text" id="irCode" placeholder="Enter IR Raw Data" size="50">
        <button onclick="sendIR()">Send IR</button>
      </body>
      </html>
    )rawliteral");
  });

  server.on("/send", HTTP_GET, []() {
    if (server.hasArg("data")) {
      String irData = server.arg("data");
      sendIRRaw(irData);
      server.send(200, "text/plain", "IR Code Sent!");
    } else {
      server.send(400, "text/plain", "Missing Data");
    }
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}

void sendIRRaw(String irData) {
  Serial.println("Received IR Data: " + irData);

  uint16_t irValues[100];  // Use uint16_t instead of int
  int index = 0;
  char* token = strtok((char*)irData.c_str(), ",");

  while (token != NULL && index < 100) {
    irValues[index++] = (uint16_t)atoi(token);  // Convert to uint16_t
    token = strtok(NULL, ",");
  }

  // Print the parsed IR values
  Serial.print("Parsed IR Values: ");
  for (int i = 0; i < index; i++) {
    Serial.print(irValues[i]);
    if (i < index - 1) {  //Only print comma if NOT the last value
      Serial.print(", ");
    }
  }
  Serial.println();     // New line after printing all values
  int carrierKhz = 38;  // Set your carrier frequency (modify if needed)
  // Send the IR signal
  IrSender.sendRaw(irValues, index, carrierKhz);  // Correct function call
  Serial.println("IR Signal Sent!");

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage
}