// MicroBSerial.h

#ifndef MICROB_SERIAL_H
#define MICROB_SERIAL_H

#include <SerialBaseVC.h>
#include <SoftwareSerial.h>

// micro:bit compatible software serial adapter.
class YFMbSoftwareSerial : public SerialBaseVC {
private:
    SoftwareSerial* _serial;

public:
    YFMbSoftwareSerial(SoftwareSerial* serial) : _serial(serial) {}

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

#endif // MICROB_SERIAL_H
