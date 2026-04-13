/*
  YFrobot VC Library - Arduino Uno 软件串口示例

  本示例演示如何在 Arduino Uno 上使用
  软件串口实现语音控制。

  硬件连接：
  语音模块        Arduino Uno
  -----------    ----------
  VCC          5V
  GND          GND
  TX           D11 (RX)
  RX           D10 (TX)

  注意：SoftwareSerial 默认使用 10（TX）和 11（RX）引脚。
  你也可以在 SoftwareSerial 构造函数中修改这些引脚。

  创建时间：2024-01-20
  作者：YFROBOT
  许可证：MIT
*/

#include <YFVCLib.h>
#include <SoftwareSerial.h>

const int VC_RX = 11; // Arduino 的 RX 引脚，连接语音模块 TX
const int VC_TX = 10; // Arduino 的 TX 引脚，连接语音模块 RX
const int LED_PIN = 13; // 板载 LED

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
  
  if (cmd != 0x01) { // 0x01 表示没有有效数据
    Serial.print("[" + String(millis()) + "] ");
    Serial.print("Voice Command: ");
    Serial.println(cmd);
    
    // 处理语音命令
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
