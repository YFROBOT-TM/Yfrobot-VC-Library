// UnoSerial.h

#ifndef UNO_SERIAL_H
#define UNO_SERIAL_H

#include "SerialBaseVC.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

// Uno硬件串口类
class UnoHardwareSerial : public SerialBaseVC {
private:
    HardwareSerial* _serial;
public:
    UnoHardwareSerial(HardwareSerial* serial) : _serial(serial) {}
    void begin(unsigned long baud) override {
        _serial->begin(baud);
    }
    uint8_t getData() override {
        uint8_t c;
        static uint8_t inBuf[3]; // 数据数组         // static 必须有，在测试ESPONE 主板时，没有无法正常工作
        static uint8_t offset;
        static uint32_t checksum;
        static enum _serial_state { IDLE, HEADER_5A } c_state;

        while (_serial->available())
        {
            c = _serial->read(); // 读串口缓冲区
            if (c_state == IDLE)
            { // 串口状态空闲,等待 HEADER 5A 状态的到来
                if (c == 0x5A)
                {                        // 判断是否进入 HEADER 5A
                    checksum = c;        // 校验和 1  -  head
                    c_state = HEADER_5A; // 进入 HEADER_5A 状态
                    offset = 0;
                }
                else
                    c_state = IDLE; // 返回IDLE状态
            }
            else if (c_state == HEADER_5A && offset < 3)
            {
                checksum += c; // 校验和 2  -  data
                inBuf[offset] = c;
                offset++;
            }
            else if (c_state == HEADER_5A && offset >= 3)
            {
                c_state = IDLE; // 返回IDLE状态
                if ((checksum & 0xFF) == c)
                {
                    return inBuf[0]; // 返回指令值
                }
            }
        }
        return 0x01; // 未接收数据，默认返回0x01
    }
};

// Uno软件串口类
class UnoSoftwareSerial : public SerialBaseVC {
private:
    SoftwareSerial* _serial;
public:
    UnoSoftwareSerial(SoftwareSerial* serial) : _serial(serial) {}
    void begin(unsigned long baud) override {
        _serial->begin(baud);
    }
    uint8_t getData() override {
        uint8_t c;
        static uint8_t inBuf[3]; // 数据数组         // static 必须有，在测试ESPONE 主板时，没有无法正常工作
        static uint8_t offset;
        static uint32_t checksum;
        static enum _serial_state { IDLE, HEADER_5A } c_state;

        while (_serial->available())
        {
            c = _serial->read(); // 读串口缓冲区
            if (c_state == IDLE)
            { // 串口状态空闲,等待 HEADER 5A 状态的到来
                if (c == 0x5A)
                {                        // 判断是否进入 HEADER 5A
                    checksum = c;        // 校验和 1  -  head
                    c_state = HEADER_5A; // 进入 HEADER_5A 状态
                    offset = 0;
                }
                else
                    c_state = IDLE; // 返回IDLE状态
            }
            else if (c_state == HEADER_5A && offset < 3)
            {
                checksum += c; // 校验和 2  -  data
                inBuf[offset] = c;
                offset++;
            }
            else if (c_state == HEADER_5A && offset >= 3)
            {
                c_state = IDLE; // 返回IDLE状态
                if ((checksum & 0xFF) == c)
                {
                    return inBuf[0]; // 返回指令值
                }
            }
        }
        return 0x01; // 未接收数据，默认返回0x01
    }
};

#endif // UNO_SERIAL_H