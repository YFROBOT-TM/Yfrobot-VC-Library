/*
  YFrobot VC Library - Arduino Uno Software Serial Example
  
  This example demonstrates voice control with Arduino Uno
  using software serial.
  
  Hardware Connections:
  Voice Module    Arduino Uno
  -----------    ----------
  VCC          5V
  GND          GND
  TX           D11 (RX)
  RX           D10 (TX)
  
  Note: SoftwareSerial uses pins 10 (TX) and 11 (RX) by default.
  You can change these pins in the SoftwareSerial constructor.
  
  Created: 2024-01-20
  Author: YFROBOT
  License: MIT
*/

#include <YFVCLib.h>
#include <SoftwareSerial.h>

const int VC_RX = 11; // Arduino RX pin, connect to voice module TX
const int VC_TX = 10; // Arduino TX pin, connect to voice module RX
const int LED_PIN = 13; // Built-in LED

SoftwareSerial softSerial(VC_RX, VC_TX);
UnoSoftwareSerial yfvc(&softSerial);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600);
  
  Serial.println("=========================================");
  Serial.println("YFrobot Voice Module - Arduino Uno Demo");
  Serial.println("(Software Serial)");
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