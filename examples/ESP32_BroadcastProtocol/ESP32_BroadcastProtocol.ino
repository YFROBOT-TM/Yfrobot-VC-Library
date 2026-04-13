#include <YFVCLib.h>

const int VC_RX_PIN = 16;
const int VC_TX_PIN = 17;
const unsigned long DEBUG_BAUD = 115200;
const unsigned long MODULE_BAUD = 9600;
const unsigned long DEMO_INTERVAL_MS = 3500;
const uint8_t DEMO_STEP_COUNT = 14;

YFESP32HardwareSerial yfvc(&Serial2, VC_RX_PIN, VC_TX_PIN);

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
      ok = yfvc.broadcastTemperature(0x15, 0x01, 0x05);
      printResult("Temp 21.15", ok);
      break;

    case 1:
      ok = yfvc.broadcastHumidity(0x1E);
      printResult("Humidity 30", ok);
      break;

    case 2:
      ok = yfvc.broadcastDate(2026, 4, 10, 5);
      printResult("Date 2026-04-10 week 5", ok);
      break;

    case 3:
      ok = yfvc.broadcastTime(0x0B, 0x12);
      printResult("Time 11:18", ok);
      break;

    case 4:
      ok = yfvc.broadcastHour(0x0A);
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
      ok = yfvc.broadcastDecimal(18, 2, 1);
      printResult("Decimal 18.21", ok);
      break;

    case 8:
      ok = yfvc.playFixedVoice(YFVC_FIXED_TEMPERATURE_PREFIX);
      printResult("Fixed 0x09", ok);
      break;

    case 9:
      ok = yfvc.playFixedVoice(YFVC_FIXED_DEGREE);
      printResult("Fixed 0x0A", ok);
      break;

    case 10:
      ok = yfvc.playFixedVoice(YFVC_FIXED_HUMIDITY_PREFIX);
      printResult("Fixed 0x0B", ok);
      break;

    case 11:
      ok = yfvc.playFixedVoice(YFVC_FIXED_CURRENT_DISTANCE);
      printResult("Fixed 0x14", ok);
      break;

    case 12:
      ok = yfvc.startCountdown10s();
      printResult("Countdown 10s", ok);
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

  Serial.println();
  Serial.println("YFrobot broadcast protocol demo for ESP32");
  Serial.println("Voice module TX -> GPIO16, RX -> GPIO17");
  Serial.println("This sketch sends the protocol examples from the V1.0.8 table.");

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
