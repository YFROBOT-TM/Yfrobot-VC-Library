// UnoSerial.h

#ifndef UNO_SERIAL_H
#define UNO_SERIAL_H

#include <SerialBaseVC.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

// Uno hardware serial adapter.
class UnoHardwareSerial : public SerialBaseVC {
private:
    HardwareSerial* _serial;

public:
    UnoHardwareSerial(HardwareSerial* serial) : _serial(serial) {}

    void begin(unsigned long baud) override {
        _serial->begin(baud);
    }

    size_t writeBytes(const uint8_t* data, size_t len) override {
        return _serial->write(data, len);
    }

    uint8_t getData() override {
        return readDataFromStream(*_serial);
    }
};

// Uno software serial adapter.
class UnoSoftwareSerial : public SerialBaseVC {
private:
    SoftwareSerial* _serial;

public:
    UnoSoftwareSerial(SoftwareSerial* serial) : _serial(serial) {}

    void begin(unsigned long baud) override {
        _serial->begin(baud);
    }

    size_t writeBytes(const uint8_t* data, size_t len) override {
        return _serial->write(data, len);
    }

    uint8_t getData() override {
        return readDataFromStream(*_serial);
    }
};

#endif // UNO_SERIAL_H
