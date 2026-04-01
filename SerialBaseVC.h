// SerialBaseVC.h

#ifndef SERIAL_BASE_VC_H
#define SERIAL_BASE_VC_H

#include <Arduino.h>

// 抽象基类，定义串口通信的接口
class SerialBaseVC {
public:
    // 纯虚函数，初始化串口
    virtual void begin(unsigned long baud) = 0;
    // 纯虚函数，读取数据
    virtual uint8_t getData() = 0;
    // 构造函数和析构函数
    SerialBaseVC() {}
    virtual ~SerialBaseVC() {}
};

#endif // SERIAL_BASE_VC_H