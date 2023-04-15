#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include "flprog_EEPROM.h"
#include "flprogI2C.h"

#define FLPROG_EEPROM_I2C_READY_STEP 0
#define FLPROG_EEPROM_I2C_NOT_READY_STEP 1

#define FLPROG_EEPROM_I2C_AFTER_WRITE_DELAY 10

class FLProgEepromI2C : public FLProgEepromBaseDevice
{
public:
    void pool();
    virtual void emergencyRecording();

protected:
    void initDevice();
    uint8_t privateWriteVar(FLProgEepromVariable *var);
    virtual void writeNeededVars();

    AbstractFLProgI2C *i2cDevice;
    uint8_t addres;
    uint32_t speed = FLPROG_I2CSPEED;
    bool isInit = false;
    uint8_t step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
    uint32_t startDelayTime;
};

class FLProg24C2X : public FLProgEepromI2C
{
public:
    FLProg24C2X(AbstractFLProgI2C *device, uint8_t i2c_address = 0x50);

protected:
    virtual uint8_t readByte(uint16_t addr);
    virtual uint8_t writeByte(uint16_t addr, uint8_t data);
};

class FLProg24C0X : public FLProgEepromI2C
{
public:
    FLProg24C0X(AbstractFLProgI2C *device, uint8_t i2c_address = 0x50);

protected:
    virtual uint8_t readByte(uint16_t addr);
    virtual uint8_t writeByte(uint16_t addr, uint8_t data);
};
