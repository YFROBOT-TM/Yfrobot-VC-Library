// SerialBaseVC.h

#ifndef SERIAL_BASE_VC_H
#define SERIAL_BASE_VC_H

#include <Arduino.h>

enum YFVCFixedVoice : uint8_t {
    YFVC_FIXED_TEMPERATURE_PREFIX = 0x09,
    YFVC_FIXED_DEGREE = 0x0A,
    YFVC_FIXED_HUMIDITY_PREFIX = 0x0B,
    YFVC_FIXED_GE = 0x0C,
    YFVC_FIXED_TEN = 0x0D,
    YFVC_FIXED_HUNDRED = 0x0E,
    YFVC_FIXED_THOUSAND = 0x0F,
    YFVC_FIXED_TEN_THOUSAND = 0x10,
    YFVC_FIXED_HUNDRED_MILLION = 0x11,
    YFVC_FIXED_DOT = 0x12,
    YFVC_FIXED_NEGATIVE = 0x13,
    YFVC_FIXED_CURRENT_DISTANCE = 0x14,
    YFVC_FIXED_MILLIMETER = 0x15,
    YFVC_FIXED_CENTIMETER = 0x16,
    YFVC_FIXED_METER = 0x17,
    YFVC_FIXED_PRESET_AUDIO = 0x19
};

// Base class for YFrobot voice module serial transport.
class SerialBaseVC {
public:
    virtual void begin(unsigned long baud) = 0;
    virtual uint8_t getData() = 0;

    // Default no-op keeps backward compatibility if a custom subclass does not add TX support.
    virtual size_t writeBytes(const uint8_t* data, size_t len) {
        (void)data;
        (void)len;
        return 0;
    }

    // Protocol V1.0.8 output frame: AA 55 + command + payload + 55 AA
    bool sendProtocolCommand(uint8_t command, const uint8_t* payload = NULL, size_t payloadLength = 0) {
        static const uint8_t kHeader[] = {0xAA, 0x55};
        static const uint8_t kFooter[] = {0x55, 0xAA};

        size_t written = 0;
        written += writeBytes(kHeader, sizeof(kHeader));
        written += writeBytes(&command, 1);

        if (payload != NULL && payloadLength > 0) {
            written += writeBytes(payload, payloadLength);
        }

        written += writeBytes(kFooter, sizeof(kFooter));
        return written == (payloadLength + sizeof(kHeader) + sizeof(kFooter) + 1);
    }

    bool broadcastTemperature(uint8_t integerPart, uint8_t decimal1 = 0, uint8_t decimal2 = 0) {
        if (integerPart > 100 || decimal1 > 9 || decimal2 > 9) {
            return false;
        }

        const uint8_t payload[] = {integerPart, decimal1, decimal2};
        return sendProtocolCommand(0x01, payload, sizeof(payload));
    }

    bool broadcastHumidity(uint8_t humidityPercent) {
        if (humidityPercent > 100) {
            return false;
        }

        return sendProtocolCommand(0x02, &humidityPercent, 1);
    }

    bool broadcastDate(uint8_t year1, uint8_t year2, uint8_t year3, uint8_t year4,
                       uint8_t month, uint8_t day, uint8_t week) {
        if (year1 > 9 || year2 > 9 || year3 > 9 || year4 > 9 ||
            month < 1 || month > 12 || day < 1 || day > 31 || week < 1 || week > 7) {
            return false;
        }

        const uint8_t payload[] = {year1, year2, year3, year4, month, day, week};
        return sendProtocolCommand(0x03, payload, sizeof(payload));
    }

    bool broadcastDate(uint16_t year, uint8_t month, uint8_t day, uint8_t week) {
        return broadcastDate(
            static_cast<uint8_t>((year / 1000U) % 10U),
            static_cast<uint8_t>((year / 100U) % 10U),
            static_cast<uint8_t>((year / 10U) % 10U),
            static_cast<uint8_t>(year % 10U),
            month,
            day,
            week
        );
    }

    bool broadcastTime(uint8_t hour, uint8_t minute) {
        if (hour > 23 || minute > 59) {
            return false;
        }

        const uint8_t payload[] = {hour, minute};
        return sendProtocolCommand(0x04, payload, sizeof(payload));
    }

    bool broadcastHour(uint8_t hour) {
        if (hour > 23) {
            return false;
        }

        return sendProtocolCommand(0x05, &hour, 1);
    }

    bool broadcastDistance(uint32_t distanceValue) {
        return sendUint32Command(0x06, distanceValue);
    }

    bool broadcastNumber(uint32_t numberValue) {
        return sendUint32Command(0x07, numberValue);
    }

    bool broadcastDecimal(uint32_t integerPart, uint8_t decimal1, uint8_t decimal2) {
        if (decimal1 > 9 || decimal2 > 9) {
            return false;
        }

        const uint8_t payload[] = {
            static_cast<uint8_t>(integerPart & 0xFF),
            static_cast<uint8_t>((integerPart >> 8) & 0xFF),
            static_cast<uint8_t>((integerPart >> 16) & 0xFF),
            static_cast<uint8_t>((integerPart >> 24) & 0xFF),
            decimal1,
            decimal2
        };
        return sendProtocolCommand(0x08, payload, sizeof(payload));
    }

    bool playFixedVoice(YFVCFixedVoice voice) {
        return sendProtocolCommand(static_cast<uint8_t>(voice));
    }

    bool startCountdown10s() {
        return sendProtocolCommand(0x18);
    }

    bool playPresetAudio() {
        return sendProtocolCommand(YFVC_FIXED_PRESET_AUDIO);
    }

    SerialBaseVC() {}
    virtual ~SerialBaseVC() {}

protected:
    bool sendUint32Command(uint8_t command, uint32_t value) {
        const uint8_t payload[] = {
            static_cast<uint8_t>(value & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
            static_cast<uint8_t>((value >> 16) & 0xFF),
            static_cast<uint8_t>((value >> 24) & 0xFF)
        };
        return sendProtocolCommand(command, payload, sizeof(payload));
    }
};

#endif // SERIAL_BASE_VC_H
