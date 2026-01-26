#pragma once
#include "Arduino.h"
#include "RT_HW_BASE.h"
#include "flprogEEPROM.h"

#define FLPROG_24C2X_EXTERNAL_EEPROM 0
#define FLPROG_24C0X_EXTERNAL_EEPROM 1

class FLProgExternalEEPROM : public FLProgAbstractEEPROM
{
public:
    FLProgExternalEEPROM(uint8_t bus, uint8_t addres, uint16_t size, uint8_t initByte, uint8_t type = FLPROG_24C2X_EXTERNAL_EEPROM);
    virtual void begin();
    virtual void pool();
    RT_HW_STRUCT_I2C_DEV *device() { return &_device; };

protected:
    uint8_t universalReadByte(uint16_t addr);
    void universalWriteByte(uint16_t addr, uint8_t value);
    uint8_t readByte24C0X(uint16_t addr);
    void writeByte24C0X(uint16_t addr, uint8_t value);
    RT_HW_STRUCT_I2C_DEV _device;
    uint8_t _type = FLPROG_24C2X_EXTERNAL_EEPROM;
    uint32_t _writeTime;
};