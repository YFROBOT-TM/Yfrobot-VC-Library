/*
  YFrobot VC Library - Arduino Uno 硬件串口示例

  本示例演示如何在 Arduino Uno 上使用
  硬件串口（Serial）实现语音控制。

  硬件连接：
  语音模块        Arduino Uno
  -----------    ----------
  VCC          5V
  GND          GND
  TX           D0 (RX)
  RX           D1 (TX)

  注意：使用硬件串口时，不能同时使用 USB
  串口监视器。本示例使用 Serial 同时进行
  语音模块通信和调试输出。

  创建时间：2024-01-20
  作者：YFROBOT
  许可证：MIT
*/

#include <YFVCLib.h>

const int LED_PIN = 13; // 板载 LED

UnoHardwareSerial yfvc(&Serial);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600);
  
  // 注意：yfvc.begin() 内部会调用 Serial.begin()
  // 调试输出会干扰语音模块通信
  
  // 启动时闪烁 LED 作为提示
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop() {
  uint8_t cmd = yfvc.getData();
  
  if (cmd != 0x01) { // 0x01 表示没有有效数据
    // 处理语音命令
    switch(cmd) {
      case 2:
        // 点亮 LED
        digitalWrite(LED_PIN, HIGH);
        break;
        
      case 3:
        // 熄灭 LED
        digitalWrite(LED_PIN, LOW);
        break;
        
      default:
        // 未知命令，闪烁一次
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        digitalWrite(LED_PIN, LOW);
        break;
    }
  }
  
  delay(10);
}
