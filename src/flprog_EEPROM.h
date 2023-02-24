#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"

class FLProgEepromVariable : public FLProgBaseVariable
{
public:
    void setAddres(uint16_t addr) { addres = addr; };
    virtual uint8_t dataSize() { return 4; };
    virtual void setBytes(uint8_t *data){};
    uint16_t getAddres() { return addres; };

protected:
    uint16_t addres;
};

class FLProgEepromByteVariable : public FLProgEepromVariable
{
public:
    uint8_t getValue() { return value; };
    FLProgEepromByteVariable(uint8_t startValue = 0) { value = startValue; };
    virtual uint8_t dataSize() { return 1; };
    virtual void setBytes(uint8_t *data);

protected:
    uint8_t value;
};

class FLProgEepromIntVariable : public FLProgEepromVariable
{
public:
    int16_t getValue() { return value; };
    FLProgEepromIntVariable(int16_t startValue = 0) { value = startValue; };
    virtual uint8_t dataSize() { return 2; };
    virtual void setBytes(uint8_t *data);

protected:
    int16_t value;
};

class FLProgEeprom4BytesVariable : public FLProgEepromVariable
{
public:
    virtual void setBytes(uint8_t *data);

protected:
    uint8_t value[4];
};

class FLProgEepromLongVariable : public FLProgEeprom4BytesVariable
{
public:
    FLProgEepromLongVariable(long startValue = 0);
    long getValue();
};

class FLProgEepromUnLongVariable : public FLProgEeprom4BytesVariable
{
public:
    unsigned long getValue();
    FLProgEepromUnLongVariable(unsigned long startValue = 0);
};

class FLProgEepromFloatVariable : public FLProgEeprom4BytesVariable
{
public:
    float getValue();
    FLProgEepromFloatVariable(float startValue = 0);
};

/*
class FLProgEepromBoolVariable : public FLProgEepromVariable
{
public:
    bool getValue() { return value; };

protected:
    bool value;
};


class FLProgEepromBoolCollector : public FLProgEepromByteVariable
{
public:
protected:
    FLProgEepromBoolVariable *vars[8];
};



/*
class FLProgEepromStringVariable : public FLProgEepromVariable
{
public:
    String getValue() { return Sting(value); };
    FLProgEepromStringVariable(char *valArray ='');

protected:
    char *value;
};
*/
class FLProgEepromBaseDevice
{
public:
    void setVars(FLProgEepromVariable *varsTable[], uint16_t tableSyze, uint8_t controlIndex = 1);
    virtual uint8_t readByte(uint16_t addr) { return 0; };
    virtual void emergencyRecording(){};
    bool isError() { return codeError; };
    virtual uint8_t readBytes(uint16_t addr, uint8_t *data, uint8_t size) { return 0; };

protected:
    void readToVariable(FLProgEepromVariable *var);
    FLProgEepromVariable **vars;
    uint16_t varSize;
    uint8_t codeError = FLPROG_SENSOR_NOT_ERROR;
};

#include "flprog_EEPROM_I2C.h"