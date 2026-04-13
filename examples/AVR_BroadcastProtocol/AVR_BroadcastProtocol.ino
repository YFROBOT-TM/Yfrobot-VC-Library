#include <YFVCLib.h>
#include <SoftwareSerial.h>

const uint8_t VC_RX_PIN = 11;
const uint8_t VC_TX_PIN = 10;
const unsigned long DEBUG_BAUD = 115200;
const unsigned long MODULE_BAUD = 9600;
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
      // AA 55 01 15 01 05 55 AA
      ok = yfvc.broadcastTemperature(0x15, 0x01, 0x05);
      printResult(F("Temp 21.15"), ok);
      break;

    case 1:
      // AA 55 02 1E 55 AA
      ok = yfvc.broadcastHumidity(0x1E);
      printResult(F("Humidity 30"), ok);
      break;

    case 2:
      // AA 55 03 02 00 02 06 04 0A 05 55 AA
      ok = yfvc.broadcastDate(2026, 4, 10, 5);
      printResult(F("Date 2026-04-10 week 5"), ok);
      break;

    case 3:
      // AA 55 04 0B 12 55 AA
      ok = yfvc.broadcastTime(0x0B, 0x12);
      printResult(F("Time 11:18"), ok);
      break;

    case 4:
      // AA 55 05 0A 55 AA
      ok = yfvc.broadcastHour(0x0A);
      printResult(F("Hour 10"), ok);
      break;

    case 5:
      // AA 55 06 24 00 00 00 55 AA
      ok = yfvc.broadcastDistance(36);
      printResult(F("Distance 36"), ok);
      break;

    case 6:
      // AA 55 07 24 11 00 00 55 AA
      ok = yfvc.broadcastNumber(4388);
      printResult(F("Number 4388"), ok);
      break;

    case 7:
      // AA 55 08 12 00 00 00 02 01 55 AA
      ok = yfvc.broadcastDecimal(18, 2, 1);
      printResult(F("Decimal 18.21"), ok);
      break;

    case 8:
      ok = yfvc.playFixedVoice(YFVC_FIXED_TEMPERATURE_PREFIX);
      printResult(F("Fixed 0x09"), ok);
      break;

    case 9:
      ok = yfvc.playFixedVoice(YFVC_FIXED_DEGREE);
      printResult(F("Fixed 0x0A"), ok);
      break;

    case 10:
      ok = yfvc.startCountdown10s();
      printResult(F("Countdown 10s"), ok);
      break;

    default:
      ok = yfvc.playPresetAudio();
      printResult(F("Preset audio"), ok);
      break;
  }
}

void setup() {
  Serial.begin(DEBUG_BAUD);
  yfvc.begin(MODULE_BAUD);

  Serial.println();
  Serial.println(F("YFrobot broadcast protocol demo for AVR"));
  Serial.println(F("Voice module TX -> D11, RX -> D10"));
  Serial.println(F("This sketch sends the protocol examples from the V1.0.8 table."));

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
