/*
  YFrobot VC Library - ESP32 Basic Example
  
  This example demonstrates basic usage of the YFrobot offline voice
  recognition module with ESP32.
  
  Hardware Connections:
  Voice Module    ESP32
  -----------    -----
  VCC          3.3V/5V
  GND          GND
  TX           GPIO16 (RX)
  RX           GPIO17 (TX)
  
  Created: 2024-01-20
  Author: YFROBOT
  License: MIT
*/

#include <YFVCLib.h>

const int VC_RX = 16; // ESP32 RX pin, connect to voice module TX
const int VC_TX = 17; // ESP32 TX pin, connect to voice module RX
const int LED_PIN = 2; // Built-in LED

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600); // Match voice module baud rate
  
  Serial.println("=========================================");
  Serial.println("YFrobot Voice Module - ESP32 Basic Demo");
  Serial.println("=========================================");
  Serial.println("Waiting for voice commands...");
  Serial.println();
}

void loop() {
  uint8_t cmd = yfvc.getData();
  
  if (cmd != 0x01) { // 0x01 = no valid data
    Serial.print("[" + String(millis()) + "] ");
    Serial.print("Voice Command: ");
    Serial.println(cmd);
    
    // Handle voice commands
    switch(cmd) {
      case 2:
        Serial.println("  -> Action: Turn ON LED");
        digitalWrite(LED_PIN, HIGH);
        break;
        
      case 3:
        Serial.println("  -> Action: Turn OFF LED");
        digitalWrite(LED_PIN, LOW);
        break;
        
      default:
        Serial.println("  -> Action: Unknown command");
        break;
    }
    Serial.println();
  }
  
  delay(10);
}