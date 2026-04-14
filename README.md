# YFrobot VC Library

[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-blue)](https://www.arduino.cc/reference/en/libraries/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

Arduino library for YFrobot offline voice modules. It supports both voice command recognition input and V1.0.9 serial broadcast output on AVR, ESP32, RP2040/Pico, SAMD, and micro:bit compatible platforms.

## Features

- Voice recognition frame parsing with checksum verification
- Serial broadcast protocol V1.0.9 output helpers
- AVR hardware serial and software serial support
- ESP32 hardware serial support with configurable pins
- RP2040 / Raspberry Pi Pico hardware serial support
- Ready-to-run recognition and broadcast examples

## Installation

### Arduino Library Manager

1. Open Arduino IDE.
2. Go to **Sketch** -> **Include Library** -> **Manage Libraries**.
3. Search for `YFrobot VC Library`.
4. Click **Install**.

### Manual Installation

1. Download the latest release from [GitHub Releases](https://github.com/yfrobot/YFrobot-VC-Library/releases).
2. Extract the ZIP file.
3. Copy the `Yfrobot-VC-Library` folder to your Arduino `libraries` folder.
4. Restart Arduino IDE.

## Wiring

### ESP32

```text
Voice Module    ESP32
-----------    -----
VCC            3.3V/5V
GND            GND
TX             GPIO16 (RX)
RX             GPIO17 (TX)
```

### Arduino Uno SoftwareSerial

```text
Voice Module    Arduino Uno
-----------    -----------
VCC            5V
GND            GND
TX             D11 (RX)
RX             D10 (TX)
```

### Arduino Uno HardwareSerial

```text
Voice Module    Arduino Uno
-----------    -----------
VCC            5V
GND            GND
TX             D0 (RX)
RX             D1 (TX)
```

### Raspberry Pi Pico

```text
Voice Module    Raspberry Pi Pico
-----------    ------------------
VCC            3.3V/5V
GND            GND
TX             Pico RX (for example GPIO9)
RX             Pico TX (for example GPIO8)
```

The exact Pico UART pins can differ between Arduino cores and remapping settings. The provided examples use `Serial1` with configurable pins.

Example:

```cpp
YFRP2040HardwareSerial yfvc(&Serial1, 9, 8); // RX, TX
```

This constructor style is intended for RP2040 cores that support `Serial1.setRX(pin)` and `Serial1.setTX(pin)` before `begin()`.

## Quick Start

### Recognition Example

```cpp
#include <YFVCLib.h>

const int VC_RX = 16;
const int VC_TX = 17;

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  yfvc.begin(9600);
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) {
    Serial.print("Voice cmd: ");
    Serial.println(cmd);
  }
}
```

### Broadcast Example

```cpp
#include <YFVCLib.h>

YFESP32HardwareSerial yfvc(&Serial2, 16, 17);

void setup() {
  yfvc.begin(9600);

  yfvc.broadcastTemperature(21.15);    // 21.15
  delay(500);
  yfvc.broadcastHumidity(30);          // 30%
  delay(500);
  yfvc.broadcastDate(2026, 4, 10, 5);  // 2026-04-10, week 5
  delay(500);
  yfvc.broadcastTime(11, 18);          // 11:18
}

void loop() {
}
```

## API

### Transport Classes

- `YFESP32HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)`
- `UnoHardwareSerial(HardwareSerial* serial)`
- `UnoSoftwareSerial(SoftwareSerial* serial)`

### Common Methods

- `begin(unsigned long baud)`
- `getData()`
- `peekData()`
- `clearData()`
- `sendProtocolCommand(uint8_t command, const uint8_t* payload = NULL, size_t payloadLength = 0)`

`getData()` refreshes serial recognition data.
- When a new voice command is recognized, it returns that command value and stores it in the internal cache.
- When there is no new recognition result, it returns `0x01`, which is `SerialBaseVC::NO_DATA`.

`peekData()` reads the cached command without reading the serial port again. This is useful for graphical or block-based programming where the same command may be checked multiple times in one logic branch.

`clearData()` clears the current cached command. Call it after the action has been handled to make sure the same voice command runs only once.

Recommended pattern for graphical programming:

```cpp
yfvc.getData();

if (yfvc.peekData() != SerialBaseVC::NO_DATA) {
  if (yfvc.peekData() == 0x02) {
    // Handle command 0x02
  } else if (yfvc.peekData() == 0x03) {
    // Handle command 0x03
  }

  yfvc.clearData();
}
```

### Broadcast Helpers

- `broadcastTemperature(double value)`
- `broadcastTemperatureParts(uint8_t integerPart, uint8_t decimal1 = 0, uint8_t decimal2 = 0)`
- `broadcastHumidity(uint8_t humidityPercent)`
- `broadcastDate(uint16_t year, uint8_t month, uint8_t day, uint8_t week)`
- `broadcastTime(uint8_t hour, uint8_t minute)`
- `broadcastTime(const tm& timeInfo)`
- `broadcastTime(rtcDateTime)`
- `broadcastHour(uint8_t hour)`
- `broadcastDistance(uint32_t distanceValue)`
- `broadcastNumber(uint32_t numberValue)`
- `broadcastDecimal(double value)`
- `broadcastDecimalParts(uint32_t integerPart, uint8_t decimal1, uint8_t decimal2)`
- `playFixedVoice(YFVCFixedVoice voice)`
- `startCountdown10s()`
- `playPresetAudio()`

`broadcastTemperature(15.51)` and `broadcastDecimal(18.21)` automatically split the value into integer part plus two decimal digits before sending the protocol frame.

`broadcastTime(...)` can be used with:

- Hour/minute integers such as `broadcastTime(11, 18)`
- Standard `struct tm`
- Common RTC objects that provide `hour()` and `minute()` methods, such as RTClib `DateTime`

### Fixed Voice IDs

`YFVCFixedVoice` maps to protocol commands `0x09` to `0x19`, including:

- `YFVC_FIXED_TEMPERATURE_PREFIX`
- `YFVC_FIXED_DEGREE`
- `YFVC_FIXED_HUMIDITY_PREFIX`
- `YFVC_FIXED_CURRENT_DISTANCE`
- `YFVC_FIXED_MILLIMETER`
- `YFVC_FIXED_CENTIMETER`
- `YFVC_FIXED_METER`
- `YFVC_FIXED_PRESET_AUDIO`

## Protocol Summary

### Recognition Input Frame

The module sends recognition data as:

```text
5A CMD DATA1 DATA2 CHECKSUM
```

`CHECKSUM` is the low 8 bits of the sum of the first four bytes.

### Broadcast Output Frame

The controller sends broadcast commands as:

```text
AA 55 CMD PAYLOAD... 55 AA
```

Examples:

- Temperature: `AA 55 01 temp_int dec1 dec2 55 AA`
- Humidity: `AA 55 02 humidity 55 AA`
- Date: `AA 55 03 y1 y2 y3 y4 month day week 55 AA`
- Time: `AA 55 04 hour minute 55 AA`
- Integer: `AA 55 07 b0 b1 b2 b3 55 AA`
- Decimal: `AA 55 08 b0 b1 b2 b3 dec1 dec2 55 AA`

For distance, integer, and decimal integer parts, the 32-bit value is little-endian: low byte first, high byte last.

## Examples

- `ESP32_Basic`: Basic ESP32 voice recognition example
- `ESP32_GraphicMode`: ESP32 cached-command example for graphical programming
- `ESP32_OLED`: ESP32 voice recognition with OLED display
- `AVR_SoftwareSerial`: Arduino Uno voice recognition over SoftwareSerial
- `AVR_PeekData`: Arduino Uno cached-command example based on `peekData()`
- `AVR_HardwareSerial`: Arduino Uno voice recognition over HardwareSerial
- `ESP32_BroadcastProtocol`: ESP32 broadcast protocol example
- `AVR_BroadcastProtocol`: AVR broadcast protocol example
- `Pico_Recognition`: Raspberry Pi Pico voice recognition example
- `Pico_BroadcastProtocol`: Raspberry Pi Pico broadcast protocol example

Open them from Arduino IDE via **File** -> **Examples** -> **YFrobot VC Library**.

## Troubleshooting

### Recognition does not work

- Make sure TX and RX are crossed.
- Confirm the module baud rate matches `begin(...)`.
- Check power and common ground.

### Broadcast does not work

- Confirm the module firmware supports protocol table V1.0.9.
- Verify the frame bytes with a serial analyzer if needed.
- Make sure the payload ranges are valid, such as month `1-12` and decimal digits `0-9`.

## Changelog

### Version 1.1.0 (2026-04-13)

- Added serial broadcast protocol V1.0.9 helper APIs
- Added AVR and ESP32 broadcast protocol examples
- Updated documentation for recognition and broadcast usage

### Version 1.0.0 (2024-01-20)

- Initial release
- Support for AVR, ESP32, and micro:bit platforms
- Hardware and software serial recognition support

## License

MIT License. See [LICENSE](LICENSE).
