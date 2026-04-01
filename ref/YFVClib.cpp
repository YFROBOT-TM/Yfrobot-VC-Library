/**************************************************************************/
/**************************************************************************/

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(ARDUINO_ARCH_SAMD)
// nothing special needed
#elif defined(ARDUINO_SAM_DUE)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#include "YFVClib.h"

// YFVClib::YFVClib(int pin_rx, int pin_tx, int baud) : _pin_rx(pin_rx), _pin_tx(pin_tx), _baud(baud), yfvcSoftSerial(pin_rx, pin_tx) {
YFVClib::YFVClib(int pin_rx, int pin_tx, int baud) {
    // 在构造函数中完成其他初始化工作

    // 保存引脚号到私有成员变量
    this->_pin_rx = pin_rx;
    this->_pin_tx = pin_tx;

    // 初始化串口通信设置
    // 这可能包括设置引脚为输入/输出，初始化通信速率等
    // pinMode(this->_pin_rx, INPUT);
    // pinMode(this->_pin_tx, OUTPUT);
    // yfvcSoftSerial.begin(this->_pin_rx, this->_pin_tx );
    this->_baud = baud;
    // yfvcSoftSerial.begin(_baud);
    
    pinMode(pintestyf, OUTPUT);
    digitalWrite(pintestyf, HIGH);
#ifdef yfvcHSerial

    // pinMode(pintest, OUTPUT);
    // digitalWrite(pintest, HIGH);
//   _hardSerial = &Serial;  // 或者选择使用 Serial，具体根据需求调整
  Serial.begin(_baud);
// #endif

// #ifdef yfvcHSerialESP

#elif defined(yfvcHSerialESP)
    // pinMode(5, OUTPUT);
    // digitalWrite(5, HIGH);
  _hardSerial = &Serial1;  // ESP32 选择使用 Serial1，具体根据需求调整
  _hardSerial->begin(_baud, this->_pin_rx, this->_pin_tx);
// #endif
// #ifndef yfvcHSerial && yfvcHSerialESP
#else

    // pinMode(9, OUTPUT);
    // digitalWrite(9, HIGH);
  _softSerial = new SoftwareSerial(this->_pin_rx,  this->_pin_tx);
  _softSerial->begin(_baud);
#endif
}

/*!
    @brief  离线语音串口接收程序，默认返回1
    @param null
*/
uint8_t YFVClib::getData() {
    uint8_t c;
    static uint8_t inBuf[3];   //数据数组         // static 必须有，在测试ESPONE 主板时，没有无法正常工作
    static uint8_t offset;
    static uint32_t checksum;
    static enum _serial_state { IDLE, HEADER_5A } c_state;
#ifdef yfvcHSerial 
    while (Serial->alavailable()) {
        c = Serial->read();     // 读串口缓冲区
#elif defined(yfvcHSerialESP)
    while (_hardSerial->alavailable()) {
        c = _hardSerial->read();     // 读串口缓冲区
#else
    while (_softSerial->available()) {
        c = _softSerial->read();     // 读串口缓冲区
#endif
        if (c_state == IDLE) {        // 串口状态空闲,等待 HEADER 5A 状态的到来
            if (c == 0x5A) {            // 判断是否进入 HEADER 5A
                checksum = c;             // 校验和 1  -  head
                c_state = HEADER_5A;      // 进入 HEADER_5A 状态
                offset = 0;
            } else  c_state = IDLE;     // 返回IDLE状态
        } else if (c_state == HEADER_5A && offset < 3) {
            checksum += c;              // 校验和 2  -  data
            inBuf[offset] = c;
            offset++;
        } else if (c_state == HEADER_5A && offset >= 3) {
            c_state = IDLE;             // 返回IDLE状态
            if ((checksum & 0xFF) == c) {
                return inBuf[0];          // 返回指令值
            }
        }
    }
    return 0x01;                    // 未接收数据，默认返回0x01
}


