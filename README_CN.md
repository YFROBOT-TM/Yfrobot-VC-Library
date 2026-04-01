# YFrobot VC Library

[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-blue)](https://www.arduino.cc/reference/en/libraries/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

用于 YFrobot 离线语音识别模块的 Arduino 库。提供多种平台的串口通信适配器，包括 AVR、ESP32 和 micro:bit。

## 功能特性

- **多平台支持**：AVR、ESP32、SAMD、micro:bit
- **硬件与软件串口**：灵活的串口通信选项
- **易于使用**：只需两个方法的简单 API
- **健壮的协议**：内置校验和验证，确保数据传输可靠
- **轻量级**：最小的内存占用

## 安装

### Arduino 库管理器
1. 打开 Arduino IDE
2. 进入 **工具** -> **管理库** (或 **Sketch** -> **Include Library** -> **Manage Libraries**)
3. 搜索 "YFrobot VC Library"
4. 点击 **安装**

### 手动安装
1. 从 [GitHub Releases](https://github.com/yfrobot/YFrobot-VC-Library/releases) 下载最新版本
2. 解压 ZIP 文件
3. 将 `Yfrobot-VC-Library` 文件夹复制到 Arduino 的 `libraries` 文件夹
   - Windows: `Documents/Arduino/libraries/`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
4. 重启 Arduino IDE

## 硬件连接

### ESP32
```
语音模块      ESP32
-----------    -----
VCC          3.3V/5V
GND          GND
TX           RX (例如 GPIO16)
RX           TX (例如 GPIO17)
```

### Arduino Uno (软件串口)
```
语音模块      Arduino Uno
-----------    ----------
VCC          5V
GND          GND
TX           RX (例如 D11)
RX           TX (例如 D10)
```

### Arduino Uno (硬件串口)
```
语音模块      Arduino Uno
-----------    ----------
VCC          5V
GND          GND
TX           RX (D0)
RX           TX (D1)
```

## 快速开始

### ESP32 示例

```cpp
#include <YFVCLib.h>

const int VC_RX = 16; // ESP32 RX 引脚
const int VC_TX = 17; // ESP32 TX 引脚

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);

void setup() {
  Serial.begin(115200);
  yfvc.begin(9600); // 与语音模块波特率一致
  Serial.println("YF 语音模块演示");
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) { // 0x01 表示无有效数据
    Serial.print("语音指令: ");
    Serial.println(cmd);
    
    // 处理指令
    switch(cmd) {
      case 2:
        Serial.println("  -> 动作: 打开 LED");
        break;
      case 3:
        Serial.println("  -> 动作: 关闭 LED");
        break;
    }
  }
  delay(10);
}
```

### Arduino Uno (软件串口)

```cpp
#include <YFVCLib.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(11, 10); // RX, TX
UnoSoftwareSerial yfvc(&softSerial);

void setup() {
  Serial.begin(9600);
  yfvc.begin(9600);
  Serial.println("YF 语音模块演示");
}

void loop() {
  uint8_t cmd = yfvc.getData();
  if (cmd != 0x01) {
    Serial.print("语音指令: ");
    Serial.println(cmd);
  }
}
```

### Arduino Uno (硬件串口)

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
    Serial.print("语音指令: ");
    Serial.println(cmd);
  }
}
```

## API 参考

### 类

#### `YFESP32HardwareSerial` (ESP32)
ESP32 硬件串口，可配置 RX/TX 引脚。

**构造函数:**
```cpp
YFESP32HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)
```

**参数:**
- `serial`: HardwareSerial 对象指针（例如 `&Serial1`、`&Serial2`）
- `rxPin`: RX 引脚编号
- `txPin`: TX 引脚编号

#### `UnoHardwareSerial` (AVR)
Arduino 硬件串口。

**构造函数:**
```cpp
UnoHardwareSerial(HardwareSerial* serial)
```

**参数:**
- `serial`: HardwareSerial 对象指针（例如 `&Serial`）

#### `UnoSoftwareSerial` (AVR)
Arduino 软件串口。

**构造函数:**
```cpp
UnoSoftwareSerial(SoftwareSerial* serial)
```

**参数:**
- `serial`: SoftwareSerial 对象指针

### 方法

#### `begin(unsigned long baud)`
初始化串口通信。

**参数:**
- `baud`: 波特率（例如 9600、115200）

#### `getData()`
从串口读取语音指令。

**返回值:**
- `uint8_t`: 指令值
  - `0x01`: 未收到有效数据
  - 其他值: 语音指令代码（请参考您的语音模块文档）

## 协议详解

语音模块发送的数据格式如下：

| 字节 | 说明 |
|------|------|
| 1    | 帧头 (0x5A) |
| 2    | 指令代码 |
| 3    | 数据字节 1 |
| 4    | 数据字节 2 |
| 5    | 校验和 (字节1-4之和，取低8位) |

库会自动处理协议解析和校验和验证。

## 示例

库包含多个示例：
- `ESP32_Basic`: ESP32 基础使用
- `ESP32_OLED`: ESP32 配合 OLED 显示
- `AVR_SoftwareSerial`: Arduino Uno 使用软件串口
- `AVR_HardwareSerial`: Arduino Uno 使用硬件串口

在 Arduino IDE 中通过 **文件** -> **示例** -> **YFrobot VC Library** 打开它们。

## 常见问题

### 无法接收数据
- 检查接线（TX/RX 应该交叉连接）
- 验证波特率是否与语音模块设置一致
- 确保共地连接

### 数据乱码
- 检查波特率设置
- 验证 RX/TX 引脚分配
- 尝试不同的波特率（9600、115200）

### ESP32 问题
- 确保使用正确的串口（Serial1、Serial2 等）
- 验证引脚编号对您的 ESP32 开发板有效
- 检查引脚是否被其他外设占用

## 许可证

本库采用 MIT 许可证。详见 [LICENSE](LICENSE)。

## 贡献

欢迎贡献！请随时提交 Pull Request。

## 支持

- 网站: [www.yfrobot.com.cn](https://www.yfrobot.com.cn)
- GitHub Issues: [在此报告问题](https://github.com/yfrobot/YFrobot-VC-Library/issues)

## 更新日志

### 版本 1.0.0 (2024-01-20)
- 首次发布
- 支持 AVR、ESP32 和 micro:bit 平台
- 硬件和软件串口实现
- 内置校验和验证

## 致谢

由 YFROBOT 开发

## 鸣谢

- U8g2 库用于 OLED 支持
- Arduino 串口示例的启发
