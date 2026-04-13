#include <YFVCLib.h>
#include <SoftwareSerial.h>

// 语音模块串口接线：
// 模块 TX -> Arduino D11
// 模块 RX -> Arduino D10
const uint8_t VC_RX_PIN = 11;
const uint8_t VC_TX_PIN = 10;

// 调试串口和模块串口波特率
const unsigned long DEBUG_BAUD = 115200;
const unsigned long MODULE_BAUD = 9600;

// 每隔一段时间轮播一条播报指令，便于逐项测试
const unsigned long DEMO_INTERVAL_MS = 3500;
const uint8_t DEMO_STEP_COUNT = 12;

SoftwareSerial moduleSerial(VC_RX_PIN, VC_TX_PIN);
UnoSoftwareSerial yfvc(&moduleSerial);

unsigned long lastDemoAt = 0;
uint8_t demoStep = 0;

void printResult(const __FlashStringHelper* title, bool ok) {
  Serial.print(title);
  Serial.println(ok ? F(" -> OK") : F(" -> FAILED"));
}

void playStep(uint8_t step) {
  bool ok = false;

  switch (step) {
    case 0:
      // 播报温度：AA 55 01 15 01 05 55 AA
      // 用户直接填写十进制温度，库内部自动拆成整数和两位小数
      ok = yfvc.broadcastTemperature(21.15);
      printResult(F("Temp 21.15"), ok);
      break;

    case 1:
      // 播报湿度：AA 55 02 1E 55 AA
      // 直接填写十进制湿度值
      ok = yfvc.broadcastHumidity(30);
      printResult(F("Humidity 30"), ok);
      break;

    case 2:
      // 播报日期：AA 55 03 02 00 02 06 04 0A 05 55 AA
      ok = yfvc.broadcastDate(2026, 4, 10, 5);
      printResult(F("Date 2026-04-10 week 5"), ok);
      break;

    case 3:
      // 播报时间：AA 55 04 0B 12 55 AA
      ok = yfvc.broadcastTime(11, 18);
      printResult(F("Time 11:18"), ok);
      break;

    case 4:
      // 整点报时：AA 55 05 0A 55 AA
      ok = yfvc.broadcastHour(10);
      printResult(F("Hour 10"), ok);
      break;

    case 5:
      // 播报距离：AA 55 06 24 00 00 00 55 AA
      ok = yfvc.broadcastDistance(36);
      printResult(F("Distance 36"), ok);
      break;

    case 6:
      // 播报整数：AA 55 07 24 11 00 00 55 AA
      ok = yfvc.broadcastNumber(4388);
      printResult(F("Number 4388"), ok);
      break;

    case 7:
      // 播报小数：AA 55 08 12 00 00 00 02 01 55 AA
      // 用户直接填写十进制小数，库内部自动拆分
      ok = yfvc.broadcastDecimal(18.21);
      printResult(F("Decimal 18.21"), ok);
      break;

    case 8:
      // 固定播报指令 0x09：当前温度是
      ok = yfvc.playFixedVoice(YFVC_FIXED_TEMPERATURE_PREFIX);
      printResult(F("Fixed 0x09"), ok);
      break;

    case 9:
      // 固定播报指令 0x0A：度
      ok = yfvc.playFixedVoice(YFVC_FIXED_DEGREE);
      printResult(F("Fixed 0x0A"), ok);
      break;

    case 10:
      // 启动 10 秒倒计时：AA 55 18 55 AA
      ok = yfvc.startCountdown10s();
      printResult(F("Countdown 10s"), ok);
      break;

    default:
      // 播放预设音乐：AA 55 19 55 AA
      ok = yfvc.playPresetAudio();
      printResult(F("Preset audio"), ok);
      break;
  }
}

void setup() {
  Serial.begin(DEBUG_BAUD);
  yfvc.begin(MODULE_BAUD);

  // 串口输出仅用于观察测试步骤，不参与模块协议通信
  Serial.println();
  Serial.println(F("YFrobot broadcast protocol demo for AVR"));
  Serial.println(F("Voice module TX -> D11, RX -> D10"));
  Serial.println(F("This sketch sends the protocol examples from the V1.0.9 table."));

  // 上电后先发送第一条示例指令
  playStep(demoStep);
  demoStep = (demoStep + 1) % DEMO_STEP_COUNT;
  lastDemoAt = millis();
}

void loop() {
  // 定时轮播后续测试项
  if (millis() - lastDemoAt >= DEMO_INTERVAL_MS) {
    playStep(demoStep);
    demoStep = (demoStep + 1) % DEMO_STEP_COUNT;
    lastDemoAt = millis();
  }
}
