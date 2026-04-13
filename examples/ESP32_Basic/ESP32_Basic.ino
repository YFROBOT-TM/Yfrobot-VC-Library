/*
  YFrobot VC Library - ESP32 基础示例

  本示例演示 YFrobot 离线语音识别模块
  在 ESP32 上的基础使用方法。

  硬件连接：
  语音模块        ESP32
  -----------    -----
  VCC          3.3V/5V
  GND          GND
  TX           GPIO16 (RX)
  RX           GPIO17 (TX)

  创建时间：2024-01-20
  作者：YFROBOT
  许可证：MIT
*/

#include <YFVCLib.h>

const int VC_RX = 16; // ESP32 的 RX 引脚，连接语音模块 TX
const int VC_TX = 17; // ESP32 的 TX 引脚，连接语音模块 RX
const int LED_PIN = 2; // 板载 LED

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600); // 与语音模块波特率保持一致
  
  Serial.println("=========================================");
  Serial.println("YFrobot Voice Module - ESP32 Basic Demo");
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
