/*
  yfrobot 离线语音模块测试- ESPONE主板

  Hardware Connections:
  ESPONE Pin  离线语音模块
  GND          GND
  VCC          VCC
  25           RX(不工作)
  26           TX

  This example code is in the public domain.
  Author     : YFROBOT ZL
  Website    : www.yfrobot.com.cn
  Create Time: 20230512
  Reference  :
*/

#include <Arduino.h>
#include "ESP32Serial.h" // 使用库中的 ESP32 串口封装类

// 根据实际接线调整 RX/TX 引脚（注意交叉连接：模块TX -> ESP32 RX）
const int VC_RX = 16; // ESP32 引脚，连接到 语音模块 TX
const int VC_TX = 17; // ESP32 引脚，连接到 语音模块 RX

// 使用 Serial1（ESP32）构造 YFESP32HardwareSerial 实例
YFESP32HardwareSerial yfvc(&Serial1, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  Serial.println("YF Voice Module ESP32 demo");
  delay(1000);
  yfvc.begin(9600); // 与语音模块的波特率一致
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) { // 0x01 表示未收到有效数据（库中约定）
    Serial.print("Voice cmd: ");
    Serial.println(cmd);
  }
  delay(10);
}