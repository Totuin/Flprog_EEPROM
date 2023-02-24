#include "flprog_EEPROM_I2C.h"

//------------------------FLProgEepromI2C-------------------
void FLProgEepromI2C::pool()
{
    if (!isInit)
    {
        initDevice();
    }
    else
    {
        if (step == FLPROG_EEPROM_I2C_NOT_READY_STEP)
        {
            if (flprog::isTimer(startDelayTime, 10))
            {
                step = FLPROG_EEPROM_I2C_READY_STEP;
            }
        }
    }
}

void FLProgEepromI2C::initDevice()
{
    if (i2cDevice->findAddr(addres))
    {
        isInit = true;
        step = FLPROG_EEPROM_I2C_READY_STEP;
        codeError = FLPROG_SENSOR_NOT_ERROR;
        return;
    }

    codeError = FLPROG_SENSOR_DEVICE_NOT_FOUND_ERROR;
    isInit = false;
    step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
}

// -- -- -- -- -- -- -- -- -FLProg24C2X-- -- -- -- -- -
FLProg24C2X::FLProg24C2X(FLProgI2C *device, uint8_t i2c_address)
{
    i2cDevice = device;
    addres = i2c_address;
}

uint8_t FLProg24C2X::readByte(uint16_t addr)
{
    if (!isInit)
    {
        initDevice();
    }
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return 0;
    }
    uint8_t data[2];
    data[0] = addr >> 8;
    data[1] = addr & 0xFF;
    codeError = i2cDevice->fullWrite(addres, data, 2);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return 0;
    }
    codeError = i2cDevice->fullRead(addres, data, 1);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return 0;
    }
    return data[0];
}

uint8_t FLProg24C2X::readBytes(uint16_t addr, uint8_t *data, uint8_t size)
{
    if (!isInit)
    {
        initDevice();
    }
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return codeError;
    }
    uint8_t addrData[2];
    addrData[0] = addr >> 8;
    addrData[1] = addr & 0xFF;
    codeError = i2cDevice->fullWrite(addres, addrData, 2);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return codeError;
    }
    codeError = i2cDevice->fullRead(addres, data, size);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
    }
    return codeError;
}

// -- -- -- -- -- -- -- -- -FLProg24C0X-- -- -- -- -- -
FLProg24C0X::FLProg24C0X(FLProgI2C *device, uint8_t i2c_address)
{
    i2cDevice = device;
    addres = i2c_address;
}

uint8_t FLProg24C0X::readBytes(uint16_t addr, uint8_t *data, uint8_t size)
{
    if (!isInit)
    {
        initDevice();
    }
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return codeError;
    }
    uint8_t addrData[1];
    codeError = i2cDevice->fullWrite(addres, (uint8_t)addr);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return codeError;
    }
    codeError = i2cDevice->fullRead(addres, data, size);
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
    }
    return codeError;
}

uint8_t FLProg24C0X::readByte(uint16_t addr)
{
    if (!isInit)
    {
        initDevice();
    }
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
        return 0;
    }
    uint8_t result = i2cDevice->fullReadReg(addres, (uint8_t)addr);
    codeError = i2cDevice->getErrorCode();
    if (codeError)
    {
        isInit = false;
        step = FLPROG_EEPROM_I2C_NOT_READY_STEP;
    }
    return result;
}
