// RP2040Serial.h

#ifndef YF_RP2040_SERIAL_H
#define YF_RP2040_SERIAL_H

#include "SerialBaseVC.h"
#include <HardwareSerial.h>

// RP2040/Pico hardware serial adapter.
//
// When using an RP2040 core that supports UART pin remapping
// (for example Arduino-Pico), rx/tx pins can be configured
// the same way as ESP32 by passing them in the constructor.
class YFRP2040HardwareSerial : public SerialBaseVC {
private:
    HardwareSerial* _serial;
    int _rxPin;
    int _txPin;

public:
    YFRP2040HardwareSerial(HardwareSerial* serial)
        : _serial(serial), _rxPin(-1), _txPin(-1) {}

    YFRP2040HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)
        : _serial(serial), _rxPin(rxPin), _txPin(txPin) {}

    void begin(unsigned long baud) override {
        if (_rxPin >= 0) {
            _serial->setRX(_rxPin);
        }
        if (_txPin >= 0) {
            _serial->setTX(_txPin);
        }
        _serial->begin(baud);
    }

    size_t writeBytes(const uint8_t* data, size_t len) override {
        return _serial->write(data, len);
    }

    uint8_t getData() override {
        uint8_t c;
        static uint8_t inBuf[3];
        static uint8_t offset;
        static uint32_t checksum;
        static enum _serial_state { IDLE, HEADER_5A } c_state;

        while (_serial->available())
        {
            c = _serial->read();
            if (c_state == IDLE)
            {
                if (c == 0x5A)
                {
                    checksum = c;
                    c_state = HEADER_5A;
                    offset = 0;
                }
                else
                {
                    c_state = IDLE;
                }
            }
            else if (c_state == HEADER_5A && offset < 3)
            {
                checksum += c;
                inBuf[offset] = c;
                offset++;
            }
            else if (c_state == HEADER_5A && offset >= 3)
            {
                c_state = IDLE;
                if ((checksum & 0xFF) == c)
                {
                    return inBuf[0];
                }
            }
        }

        return 0x01;
    }
};

#endif // YF_RP2040_SERIAL_H
