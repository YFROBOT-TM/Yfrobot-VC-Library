// ESP32Serial.h

#ifndef YF_ESP32_SERIAL_H
#define YF_ESP32_SERIAL_H

#include <SerialBaseVC.h>
#include <HardwareSerial.h>

// ESP32 hardware serial adapter.
class YFESP32HardwareSerial : public SerialBaseVC {
private:
    HardwareSerial* _serial;
    int _txPin;
    int _rxPin;

public:
    YFESP32HardwareSerial(HardwareSerial* serial, int rxPin, int txPin)
        : _serial(serial), _txPin(txPin), _rxPin(rxPin) {}

    void begin(unsigned long baud) override {
        _serial->begin(baud, SERIAL_8N1, _rxPin, _txPin);
    }

    size_t writeBytes(const uint8_t* data, size_t len) override {
        return _serial->write(data, len);
    }

    uint8_t getData() override {
        return readDataFromStream(*_serial);
    }
};

#endif // YF_ESP32_SERIAL_H
