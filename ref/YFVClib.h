/**************************************************************************/
/**************************************************************************/

#ifndef _YFVCLIB_H_
#define _YFVCLIB_H_

#include <Arduino.h>

// 根据条件选择使用硬件串口或软件串口
#ifdef yfvcHSerial
#elif defined(yfvcHSerialESP)
#include <HardwareSerial.h>
HardwareSerial *_hardSerial = nullptr; // 默认为nullptr
#else
#include <SoftwareSerial.h>
SoftwareSerial *_softSerial = nullptr; // 默认为nullptr
#endif

 
class YFVClib {
    public:

        YFVClib(int pin_rx, int pin_tx, int baud);
        uint8_t getData();
        // uint8_t getDataH();

    private:
        int _pin_rx;			//RX pin
        int _pin_tx;			//TX pin
        int _baud;			//baud
        // SoftwareSerial yfvcSoftSerial;

#ifdef yfvcHSerial
int pintestyf = 11;
#elif defined(yfvcHSerialESP)
int pintestyf = 9;
#else
int pintestyf = 5;
#endif

};

#endif // _YFVCLIB_H_
