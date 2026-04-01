/*
  YFrobot VC Library - Arduino Uno Hardware Serial Example
  
  This example demonstrates voice control with Arduino Uno
  using hardware serial (Serial).
  
  Hardware Connections:
  Voice Module    Arduino Uno
  -----------    ----------
  VCC          5V
  GND          GND
  TX           D0 (RX)
  RX           D1 (TX)
  
  Note: When using hardware serial, you cannot use the USB
  serial monitor simultaneously. This example uses Serial
  for both voice module communication and debug output.
  
  Created: 2024-01-20
  Author: YFROBOT
  License: MIT
*/

#include <YFVCLib.h>

const int LED_PIN = 13; // Built-in LED

UnoHardwareSerial yfvc(&Serial);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600);
  
  // Note: Serial.begin() is called by yfvc.begin()
  // Debug output will interfere with voice module communication
  
  // Blink LED to indicate startup
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop() {
  uint8_t cmd = yfvc.getData();
  
  if (cmd != 0x01) { // 0x01 = no valid data
    // Handle voice commands
    switch(cmd) {
      case 2:
        // Turn ON LED
        digitalWrite(LED_PIN, HIGH);
        break;
        
      case 3:
        // Turn OFF LED
        digitalWrite(LED_PIN, LOW);
        break;
        
      default:
        // Unknown command - blink once
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        digitalWrite(LED_PIN, LOW);
        break;
    }
  }
  
  delay(10);
}