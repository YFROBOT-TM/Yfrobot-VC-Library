/*
  YFrobot VC Library - ESP32 graphic mode demo

  getData() refreshes recognition data.
  peekData() reads the cached command for repeated comparisons.
  clearData() clears the cache after the action is handled.
*/

#include <YFVCLib.h>

const int VC_RX = 16;
const int VC_TX = 17;

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  yfvc.begin(9600);
}

void loop() {
  // Refresh serial recognition result once.
  yfvc.getData();
  if (yfvc.peekData() != SerialBaseVC::NO_DATA) {
    if (yfvc.peekData() == 0x02) {
      // Handle command 0x02 here.
    } else if (yfvc.peekData() == 0x03) {
      // Handle command 0x03 here.
    }

    yfvc.clearData();
  }

  delay(10);
}
