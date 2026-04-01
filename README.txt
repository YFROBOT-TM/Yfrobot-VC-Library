// ...existing code...

YFrobot VC Library - 离线语音识别模块库
======================================

简介
----
本库提供针对多种 MCU 的串口读取适配层，用于读取离线语音识别模块的指令数据。支持：
- AVR（SoftwareSerial / HardwareSerial）
- ESP32（HardwareSerial，指定 RX/TX 引脚）
- micro:bit（软件串口）

关键类型与文件
----------------
- 串口抽象基类：[`SerialBaseVC`](SerialBaseVC.h)
- ESP32 封装类：[`YFESP32HardwareSerial`](ESP32Serial.h)
- 兼容实现（参考）：[`YFVClib`](ref/YFVClib.h)
- 示例：
  - AVR software serial 示例：[examples/vc/vc.ino](examples/vc/vc.ino)
  - AVR hardware serial 示例：[examples/vc_h/vc_h.ino](examples/vc_h/vc_h.ino)
  - ESP32 示例（本 README 附带）：examples/esp32/esp32_example.ino

快速上手（ESP32）
-----------------
1. 接线（交叉）：
   - 模块 TX -> ESP32 VC_RX（示例中为 16）
   - 模块 RX -> ESP32 VC_TX（示例中为 17）
   - 共地

2. 将示例上传到 ESP32：
   - 修改 examples/esp32/esp32_example.ino 中的 VC_RX/VC_TX 为实际引脚
   - 确保波特率与模块一致（示例使用 9600）
   - 打开串口监视器（115200）查看模块返回值

库接口说明
----------
- SerialBaseVC::begin(unsigned long baud) —— 初始化串口
- SerialBaseVC::getData() -> uint8_t —— 读取一条指令，库约定：
  - 返回 0x01 表示未收到有效数据（或无完整帧）
  - 返回其他值为识别到的指令代码（详见模块说明）

更多说明与实现参考
------------------
- ESP32 实现：[`ESP32Serial.h`](ESP32Serial.h)
- 抽象基类：[`SerialBaseVC.h`](SerialBaseVC.h)
- 老版本/参考实现：[`ref/YFVClib.h`](ref/YFVClib.h) / [`ref/YFVClib.cpp`](ref/YFVClib.cpp)

示例说明
--------
- AVR 示例（软件串口）: [examples/vc/vc.ino](examples/vc/vc.ino)
- AVR 示例（硬件串口）: [examples/vc_h/vc_h.ino](examples/vc_h/vc_h.ino)
- ESP32 示例: examples/esp32/esp32_example.ino（见上方）

版本说明
--------
v1.0:
  2024/01/20 - 适配 Mind+，解决多个软件串口库冲突问题

// ...existing code...	 