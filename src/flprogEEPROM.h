#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"

#ifndef FLPROG_EEPROM
#ifdef ARDUINO_ARCH_ESP8266
#define FLPROG_EEPROM
#define FLPROG_ESP_EEPROM
#endif
#endif

#ifndef FLPROG_EEPROM
#ifdef ARDUINO_ARCH_ESP32
#define FLPROG_EEPROM
#define FLPROG_ESP_EEPROM
#endif
#endif

#ifndef FLPROG_EEPROM
#ifdef ARDUINO_ARCH_RP2040
#define FLPROG_EEPROM
#define FLPROG_RP2040_EEPROM
#endif
#endif

#ifndef FLPROG_EEPROM
#ifdef ARDUINO_ARCH_STM32
#define FLPROG_EEPROM
#define FLPROG_STM32_EEPROM
#endif
#endif

#ifndef FLPROG_EEPROM
#define FLPROG_EEPROM
#define FLPROG_AVR_EEPROM
#endif

class FLProgAbstractEEPROM
{
public:
    virtual void begin() = 0;
    virtual void pool() = 0;

    void setBooleanStartValue(uint16_t startAddres, uint8_t bit, bool value) { saveBoolean(startAddres, bit, value, false); };
    void setByteStartValue(uint16_t startAddres, uint8_t value) { saveByte(startAddres, value, false); };
    void setIntegerStartValue(uint16_t startAddres, int16_t value) { saveInteger(startAddres, value, false); };
    void setLongStartValue(uint16_t startAddres, int32_t value) { saveLong(startAddres, value, false); };
    void setUnsignedLongStartValue(uint16_t startAddres, uint32_t value) { saveUnsignedLong(startAddres, value, false); };
    void setByteArrayStartValue(uint16_t startAddres, uint16_t length, uint8_t *value) { saveByteArray(startAddres, length, value, false); };
    void setStringStartValue(uint16_t startAddres, uint16_t length, String value) { saveString(startAddres, length, value, false); };

    void saveBoolean(uint16_t startAddres, uint8_t bit, bool value, bool needUpdate = true);
    void saveByte(uint16_t startAddres, uint8_t value, bool needUpdate = true);
    void saveInteger(uint16_t startAddres, int16_t value, bool needUpdate = true);
    void saveLong(uint16_t startAddres, int32_t value, bool needUpdate = true);
    void saveUnsignedLong(uint16_t startAddres, uint32_t value, bool needUpdate = true);
    void saveString(uint16_t startAddres, uint16_t length, String value, bool needUpdate = true);
    void saveByteArray(uint16_t startAddres, uint16_t length, uint8_t *value, bool needUpdate = true);

    bool readBoolean(uint16_t startAddres, uint8_t bit);
    uint8_t readByte(uint16_t startAddres);
    int16_t readInteger(uint16_t startAddres);
    int32_t readLong(uint16_t startAddres);
    uint32_t readUnsignedLong(uint16_t startAddres);
    String readString(uint16_t startAddres, uint16_t length);
    void readByteArray(uint16_t startAddres, uint16_t length, uint8_t *value);

protected:
    virtual bool checkAddres(uint16_t addres, uint16_t endAddres);
    uint16_t nextUpdateByteAddress();
    uint16_t _size = 0;
    uint8_t *_data = 0;
    bool *_dataChanged = 0;
};
