# YFrobot VC Library

[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-blue)](https://www.arduino.cc/reference/en/libraries/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

YFrobot 离线语音模块 Arduino 库，支持两类能力：

- 语音识别结果接收与解析
- 串口播报协议 V1.0.9 指令发送

当前支持 AVR、ESP32、RP2040/Pico、SAMD 与 micro:bit 兼容平台。

## 功能特性

- 支持语音识别串口帧解析与校验
- 支持 V1.0.9 播报协议封装
- 支持 AVR 硬件串口与软件串口
- 支持 ESP32 可配置引脚的硬件串口
- 支持 RP2040 / Raspberry Pi Pico 硬件串口
- 提供识别示例与播报示例，方便快速测试

## 安装

### 通过 Arduino 库管理器安装

1. 打开 Arduino IDE。
2. 进入 **Sketch** -> **Include Library** -> **Manage Libraries**。
3. 搜索 `YFrobot VC Library`。
4. 点击安装。

### 手动安装

1. 从 [GitHub Releases](https://github.com/yfrobot/YFrobot-VC-Library/releases) 下载最新版本。
2. 解压 ZIP 文件。
3. 将 `Yfrobot-VC-Library` 文件夹复制到 Arduino 的 `libraries` 目录。
4. 重启 Arduino IDE。

## 硬件连接

### ESP32

```text
语音模块        ESP32
-----------    -----
VCC            3.3V/5V
GND            GND
TX             GPIO16 (RX)
RX             GPIO17 (TX)
```

### Arduino Uno 软件串口

```text
语音模块        Arduino Uno
-----------    -----------
VCC            5V
GND            GND
TX             D11 (RX)
RX             D10 (TX)
```

### Arduino Uno 硬件串口

```text
语音模块        Arduino Uno
-----------    -----------
VCC            5V
GND            GND
TX             D0 (RX)
RX             D1 (TX)
```

### Raspberry Pi Pico

```text
语音模块        Raspberry Pi Pico
-----------    ------------------
VCC            3.3V/5V
GND            GND
TX             Pico RX（例如 GPIO9）
RX             Pico TX（例如 GPIO8）
```

不同 Pico Arduino 核心对 UART 默认引脚和重映射方式可能略有差异，当前示例默认使用 `Serial1`，并支持像 ESP32 一样在构造函数中传入自定义 RX/TX 引脚。

示例：

```cpp
YFRP2040HardwareSerial yfvc(&Serial1, 9, 8); // RX, TX
```

该方式依赖 Pico 所使用的 Arduino core 支持在 `begin()` 前调用 `Serial1.setRX(pin)` 与 `Serial1.setTX(pin)`。

## 快速开始

### 1. 语音识别示例

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

### 2. 串口播报示例

```cpp
#include <YFVCLib.h>

YFESP32HardwareSerial yfvc(&Serial2, 16, 17);

void setup() {
  yfvc.begin(9600);

  yfvc.broadcastTemperature(21.15);    // 21.15
  delay(500);
  yfvc.broadcastHumidity(30);          // 30%
  delay(500);
  yfvc.broadcastDate(2026, 4, 10, 5);  // 2026-04-10，星期 5
  delay(500);
  yfvc.broadcastTime(11, 18);          // 11:18
}

void loop() {
}
```

## API 说明

### 传输类

- `YFESP32HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)`
- `UnoHardwareSerial(HardwareSerial* serial)`
- `UnoSoftwareSerial(SoftwareSerial* serial)`

### 基础方法

- `begin(unsigned long baud)`
- `getData()`
- `peekData()`
- `clearData()`
- `sendProtocolCommand(uint8_t command, const uint8_t* payload = NULL, size_t payloadLength = 0)`

`getData()` 用于刷新串口识别结果。
- 当识别到新的语音指令时，返回该指令值，同时把该指令写入缓存。
- 当没有新的识别结果时，返回 `0x01`，即 `SerialBaseVC::NO_DATA`。

`peekData()` 用于查看当前缓存的指令，不会再次读取串口，适合图形化/积木式编程在同一轮逻辑中多次判断。

`clearData()` 用于清除当前缓存。执行完动作后调用一次，就可以实现“同一条语音只执行一次”。

图形化编程推荐写法：

```cpp
yfvc.getData();

if (yfvc.peekData() != SerialBaseVC::NO_DATA) {
  if (yfvc.peekData() == 0x02) {
    // 处理 0x02 指令
  } else if (yfvc.peekData() == 0x03) {
    // 处理 0x03 指令
  }

  yfvc.clearData();
}
```

### 播报协议方法

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

`broadcastTemperature(15.51)` 与 `broadcastDecimal(18.21)` 会在库内部自动拆分为整数部分和两位小数，再按协议发送。

`broadcastTime(...)` 支持以下几种写法：

- 直接传入小时和分钟，例如 `broadcastTime(11, 18)`
- 传入标准 `struct tm`
- 传入带有 `hour()` 和 `minute()` 方法的常见 RTC 时间对象，例如 RTClib 的 `DateTime`

### 固定播报指令枚举

`YFVCFixedVoice` 对应协议中的固定指令，常用项包括：

- `YFVC_FIXED_TEMPERATURE_PREFIX`：`0x09`
- `YFVC_FIXED_DEGREE`：`0x0A`
- `YFVC_FIXED_HUMIDITY_PREFIX`：`0x0B`
- `YFVC_FIXED_CURRENT_DISTANCE`：`0x14`
- `YFVC_FIXED_MILLIMETER`：`0x15`
- `YFVC_FIXED_CENTIMETER`：`0x16`
- `YFVC_FIXED_METER`：`0x17`
- `YFVC_FIXED_PRESET_AUDIO`：`0x19`

## 协议说明

### 1. 识别输入协议

模块返回的识别数据格式为：

```text
5A CMD DATA1 DATA2 CHECKSUM
```

其中 `CHECKSUM` 为前四个字节之和的低 8 位。

### 2. 播报输出协议

主控发送的播报数据格式为：

```text
AA 55 CMD PAYLOAD... 55 AA
```

示例：

- 温度：`AA 55 01 temp_int dec1 dec2 55 AA`
- 湿度：`AA 55 02 humidity 55 AA`
- 日期：`AA 55 03 y1 y2 y3 y4 month day week 55 AA`
- 时间：`AA 55 04 hour minute 55 AA`
- 整数：`AA 55 07 b0 b1 b2 b3 55 AA`
- 小数：`AA 55 08 b0 b1 b2 b3 dec1 dec2 55 AA`

距离、整数、小数的整数部分均采用 4 字节小端格式，即低字节在前，高字节在后。

## 示例列表

- `ESP32_Basic`：ESP32 语音识别基础示例
- `ESP32_OLED`：ESP32 配合 OLED 显示的识别示例
- `AVR_SoftwareSerial`：Arduino Uno 软件串口识别示例
- `AVR_HardwareSerial`：Arduino Uno 硬件串口识别示例
- `ESP32_BroadcastProtocol`：ESP32 播报协议示例
- `AVR_BroadcastProtocol`：AVR 播报协议示例
- `Pico_Recognition`：Raspberry Pi Pico 语音识别示例
- `Pico_BroadcastProtocol`：Raspberry Pi Pico 播报协议示例

可在 Arduino IDE 中通过 **File** -> **Examples** -> **YFrobot VC Library** 打开。

## 常见问题

### 识别无数据

- 检查 TX/RX 是否交叉连接。
- 检查波特率是否与模块一致。
- 检查供电与共地是否正常。

### 播报无响应

- 确认模块固件支持 V1.0.9 协议表。
- 确认发送值范围合法，例如月份 `1-12`、小数位 `0-9`。
- 必要时用串口分析仪核对发送帧。

## 更新日志

### 版本 1.1.0（2026-04-13）

- 新增 V1.0.9 串口播报协议 API
- 新增 AVR 与 ESP32 播报协议示例
- 更新中英文文档与示例说明

### 版本 1.0.0（2024-01-20）

- 初次发布
- 支持 AVR、ESP32、micro:bit 平台
- 支持语音识别串口接收

## 许可证

本库采用 MIT License，详见 [LICENSE](LICENSE)。
