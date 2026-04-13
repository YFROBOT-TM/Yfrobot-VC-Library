#include <YFVCLib.h>

/*
  Raspberry Pi Pico 语音识别示例

  使用硬件串口 Serial1 与语音模块通信，
  调试信息通过 USB 串口 Serial 输出。

  本示例支持像 ESP32 一样自由指定 Pico 的 UART 引脚。
  下面的 RX/TX 引脚可按实际接线修改。

  示例接线：
  模块 TX -> Pico GPIO9  (RX)
  模块 RX -> Pico GPIO8  (TX)
  模块 VCC -> 3.3V / 5V
  模块 GND -> GND
*/

const unsigned long DEBUG_BAUD = 115200;
const unsigned long MODULE_BAUD = 9600;
const int VC_RX_PIN = 9;
const int VC_TX_PIN = 8;

YFRP2040HardwareSerial yfvc(&Serial1, VC_RX_PIN, VC_TX_PIN);

void setup() {
  Serial.begin(DEBUG_BAUD);
  yfvc.begin(MODULE_BAUD);

  delay(1000);
  Serial.println();
  Serial.println("YFrobot voice recognition demo for Raspberry Pi Pico");
  Serial.println("USB Serial = debug, Serial1 = voice module");
  Serial.println("Module TX -> GPIO9, Module RX -> GPIO8");
}

void loop() {
  uint8_t cmd = yfvc.getData();

  if (cmd != 0x01) {
    Serial.print("Voice command: ");
    Serial.println(cmd);
  }

  delay(10);
}
