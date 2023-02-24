#pragma once
#include "Arduino.h"
// #include "flprogUtilites.h"
#include "flprog_EEPROM.h"

#define FLPROG_EEPROM_I2C_READY_STEP 0
#define FLPROG_EEPROM_I2C_NOT_READY_STEP 1

class FLProgEepromI2C : public FLProgEepromBaseDevice
{
public:
    void pool();

protected:
    void initDevice();
    FLProgI2C *i2cDevice;
    uint8_t addres;
    uint32_t speed = FLPROG_I2CSPEED;
    bool isInit = false;
    uint8_t step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
    uint32_t startDelayTime;
};

class FLProg24C2X : public FLProgEepromI2C
{
public:
    FLProg24C2X(FLProgI2C *device, uint8_t i2c_address = 0x50);
    virtual uint8_t readByte(uint16_t addr);
    virtual uint8_t readBytes(uint16_t addr, uint8_t *data, uint8_t size);

protected:
};

class FLProg24C0X : public FLProgEepromI2C
{
public:
    FLProg24C0X(FLProgI2C *device, uint8_t i2c_address = 0x50);
    virtual uint8_t readByte(uint16_t addr);
    virtual uint8_t readBytes(uint16_t addr, uint8_t *data, uint8_t size);

protected:
};
