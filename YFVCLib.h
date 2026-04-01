#ifndef YFVC_LIB_H
#define YFVC_LIB_H

#include "SerialBaseVC.h"

#if defined(ARDUINO_ARCH_ESP32)
  #include "ESP32Serial.h"
  #define YFVC_HARDWARE_SERIAL YFESP32HardwareSerial
#elif defined(ARDUINO_ARCH_AVR)
  #include "UnoSerial.h"
  #define YFVC_HARDWARE_SERIAL UnoHardwareSerial
  #define YFVC_SOFTWARE_SERIAL UnoSoftwareSerial
#elif defined(ARDUINO_ARCH_SAMD)
  #include "UnoSerial.h"
  #define YFVC_HARDWARE_SERIAL UnoHardwareSerial
  #define YFVC_SOFTWARE_SERIAL UnoSoftwareSerial
#else
  #include "UnoSerial.h"
  #define YFVC_HARDWARE_SERIAL UnoHardwareSerial
  #define YFVC_SOFTWARE_SERIAL UnoSoftwareSerial
#endif

#endif // YFVC_LIB_H