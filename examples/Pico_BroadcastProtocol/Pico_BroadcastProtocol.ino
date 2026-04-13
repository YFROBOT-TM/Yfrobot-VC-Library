#include <YFVCLib.h>

/*
  Raspberry Pi Pico 串口播报协议示例

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
const unsigned long DEMO_INTERVAL_MS = 3500;
const uint8_t DEMO_STEP_COUNT = 9;
const int VC_RX_PIN = 9;
const int VC_TX_PIN = 8;

YFRP2040HardwareSerial yfvc(&Serial1, VC_RX_PIN, VC_TX_PIN);

unsigned long lastDemoAt = 0;
uint8_t demoStep = 0;

void printResult(const char* title, bool ok) {
  Serial.print(title);
  Serial.println(ok ? " -> OK" : " -> FAILED");
}

void playStep(uint8_t step) {
  bool ok = false;

  switch (step) {
    case 0:
      // 用户直接填写十进制温度，库内部自动拆分为协议需要的整数与两位小数
      ok = yfvc.broadcastTemperature(21.15);
      printResult("Temp 21.15", ok);
      break;

    case 1:
      // 湿度直接填写十进制
      ok = yfvc.broadcastHumidity(30);
      printResult("Humidity 30", ok);
      break;

    case 2:
      ok = yfvc.broadcastDate(2026, 4, 13, 1);
      printResult("Date 2026-04-13 week 1", ok);
      break;

    case 3:
      ok = yfvc.broadcastTime(11, 18);
      printResult("Time 11:18", ok);
      break;

    case 4:
      ok = yfvc.broadcastHour(10);
      printResult("Hour 10", ok);
      break;

    case 5:
      ok = yfvc.broadcastDistance(36);
      printResult("Distance 36", ok);
      break;

    case 6:
      ok = yfvc.broadcastNumber(4388);
      printResult("Number 4388", ok);
      break;

    case 7:
      ok = yfvc.broadcastDecimal(18.21);
      printResult("Decimal 18.21", ok);
      break;

    default:
      ok = yfvc.playPresetAudio();
      printResult("Preset audio", ok);
      break;
  }
}

void setup() {
  Serial.begin(DEBUG_BAUD);
  yfvc.begin(MODULE_BAUD);

  delay(1000);
  Serial.println();
  Serial.println("YFrobot broadcast protocol demo for Raspberry Pi Pico");
  Serial.println("USB Serial = debug, Serial1 = voice module");
  Serial.println("Module TX -> GPIO9, Module RX -> GPIO8");

  playStep(demoStep);
  demoStep = (demoStep + 1) % DEMO_STEP_COUNT;
  lastDemoAt = millis();
}

void loop() {
  if (millis() - lastDemoAt >= DEMO_INTERVAL_MS) {
    playStep(demoStep);
    demoStep = (demoStep + 1) % DEMO_STEP_COUNT;
    lastDemoAt = millis();
  }
}
