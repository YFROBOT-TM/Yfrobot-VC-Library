// RP2040Serial.h

#ifndef YF_RP2040_SERIAL_H
#define YF_RP2040_SERIAL_H

#include <SerialBaseVC.h>
#include <HardwareSerial.h>

// RP2040/Pico hardware serial adapter.
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
        return readDataFromStream(*_serial);
    }
};

#endif // YF_RP2040_SERIAL_H
