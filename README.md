# YFrobot VC Library

[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-blue)](https://www.arduino.cc/reference/en/libraries/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

Arduino library for YFrobot offline voice recognition modules. Provides serial communication adapters for multiple platforms including AVR, ESP32, and micro:bit.

## Features

- **Multi-platform support**: AVR, ESP32, SAMD, micro:bit
- **Hardware & Software Serial**: Flexible serial communication options
- **Easy to use**: Simple API with just two methods
- **Robust protocol**: Built-in checksum verification for reliable data transmission
- **Lightweight**: Minimal memory footprint

## Installation

### Arduino Library Manager
1. Open Arduino IDE
2. Go to **Sketch** -> **Include Library** -> **Manage Libraries**
3. Search for "YFrobot VC Library"
4. Click **Install**

### Manual Installation
1. Download the latest release from [GitHub Releases](https://github.com/yfrobot/YFrobot-VC-Library/releases)
2. Extract the ZIP file
3. Copy the `Yfrobot-VC-Library` folder to your Arduino `libraries` folder
   - Windows: `Documents/Arduino/libraries/`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
4. Restart Arduino IDE

## Hardware Connections

### ESP32
```
Voice Module    ESP32
-----------    -----
VCC          3.3V/5V
GND          GND
TX           RX (e.g., GPIO16)
RX           TX (e.g., GPIO17)
```

### Arduino Uno (Software Serial)
```
Voice Module    Arduino Uno
-----------    ----------
VCC          5V
GND          GND
TX           RX (e.g., D11)
RX           TX (e.g., D10)
```

### Arduino Uno (Hardware Serial)
```
Voice Module    Arduino Uno
-----------    ----------
VCC          5V
GND          GND
TX           RX (D0)
RX           TX (D1)
```

## Quick Start

### ESP32 Example

```cpp
#include <YFVCLib.h>

const int VC_RX = 16; // ESP32 RX pin
const int VC_TX = 17; // ESP32 TX pin

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  yfvc.begin(9600); // Match voice module baud rate
  Serial.println("YF Voice Module Demo");
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) { // 0x01 = no valid data
    Serial.print("Voice cmd: ");
    Serial.println(cmd);
    
    // Handle commands
    switch(cmd) {
      case 2:
        Serial.println("Turn on LED");
        break;
      case 3:
        Serial.println("Turn off LED");
        break;
    }
  }
  delay(10);
}
```

### Arduino Uno (Software Serial)

```cpp
#include <YFVCLib.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(11, 10); // RX, TX
UnoSoftwareSerial yfvc(&softSerial);

void setup() {
  Serial.begin(9600);
  yfvc.begin(9600);
  Serial.println("YF Voice Module Demo");
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) {
    Serial.print("Voice cmd: ");
    Serial.println(cmd);
  }
}
```

### Arduino Uno (Hardware Serial)

```cpp
#include <YFVCLib.h>

UnoHardwareSerial yfvc(&Serial);

void setup() {
  Serial.begin(9600);
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

## API Reference

### Classes

#### `YFESP32HardwareSerial` (ESP32)
ESP32 hardware serial with configurable RX/TX pins.

**Constructor:**
```cpp
YFESP32HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)
```

**Parameters:**
- `serial`: Pointer to HardwareSerial object (e.g., `&Serial1`, `&Serial2`)
- `rxPin`: RX pin number
- `txPin`: TX pin number

#### `UnoHardwareSerial` (AVR)
Arduino hardware serial.

**Constructor:**
```cpp
UnoHardwareSerial(HardwareSerial* serial)
```

**Parameters:**
- `serial`: Pointer to HardwareSerial object (e.g., `&Serial`)

#### `UnoSoftwareSerial` (AVR)
Arduino software serial.

**Constructor:**
```cpp
UnoSoftwareSerial(SoftwareSerial* serial)
```

**Parameters:**
- `serial`: Pointer to SoftwareSerial object

### Methods

#### `begin(unsigned long baud)`
Initialize serial communication.

**Parameters:**
- `baud`: Baud rate (e.g., 9600, 115200)

#### `getData()`
Read voice command from serial port.

**Returns:**
- `uint8_t`: Command value
  - `0x01`: No valid data received
  - Other values: Voice command code (refer to your voice module documentation)

## Protocol Details

The voice module sends data in the following format:

| Byte | Description |
|------|-------------|
| 1    | Header (0x5A) |
| 2    | Command code |
| 3    | Data byte 1 |
| 4    | Data byte 2 |
| 5    | Checksum (sum of bytes 1-4, lower 8 bits) |

The library automatically handles protocol parsing and checksum verification.

## Examples

The library includes several examples:
- `ESP32_Basic`: Basic ESP32 usage
- `ESP32_OLED`: ESP32 with OLED display
- `AVR_SoftwareSerial`: Arduino Uno with software serial
- `AVR_HardwareSerial`: Arduino Uno with hardware serial

Open them in Arduino IDE via **File** -> **Examples** -> **YFrobot VC Library**.

## Troubleshooting

### No data received
- Check wiring (TX/RX should be crossed)
- Verify baud rate matches voice module setting
- Ensure common ground connection

### Garbled data
- Check baud rate settings
- Verify RX/TX pin assignments
- Try different baud rates (9600, 115200)

### ESP32 issues
- Ensure correct Serial port (Serial1, Serial2, etc.)
- Verify pin numbers are valid for your ESP32 board
- Check that pins are not used by other peripherals

## License

This library is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

- Website: [www.yfrobot.com.cn](https://www.yfrobot.com.cn)
- GitHub Issues: [Report issues here](https://github.com/yfrobot/YFrobot-VC-Library/issues)

## Changelog

### Version 1.0.0 (2024-01-20)
- Initial release
- Support for AVR, ESP32, and micro:bit platforms
- Hardware and software serial implementations
- Built-in checksum verification

## Credits

Developed by YFROBOT

## Acknowledgments

- Based on the U8g2 library for OLED support
- Inspired by Arduino Serial examples