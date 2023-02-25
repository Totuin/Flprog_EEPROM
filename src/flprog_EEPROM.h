#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"

class FLProgEepromVariable : public FLProgBaseVariable
{
public:
    uint16_t getAddres() { return addres; };
    virtual void setIsNeedWrite(bool value) { isNeededWrite = value; };
    virtual bool getIsNeededWrite() { return isNeededWrite; };
    virtual uint8_t getWriteByte() { return (getBytes())[pointer]; };
    virtual uint16_t getWriteAddress() { return addres + pointer; };
    virtual void next();
    virtual uint8_t dataSize() { return 4; };
    virtual uint8_t *getBytes() { return 0; };
    virtual void setAddres(uint16_t addr) { addres = addr; };
    virtual void setByte(uint8_t index, uint8_t value) { (getBytes())[index] = value; };

protected:
    uint16_t addres;
    uint8_t pointer = 0;
    bool isNeededWrite = false;
};

class FLProgEepromByteVariable : public FLProgEepromVariable
{
public:
    FLProgEepromByteVariable(uint8_t startValue = 0) { value[0] = startValue; };
    uint8_t getValue() { return value[0]; };
    void setValue(uint8_t newVal);

    virtual uint8_t dataSize() { return 1; };
    virtual uint8_t *getBytes() { return value; };

protected:
    uint8_t value[1];
};

class FLProgEepromIntVariable : public FLProgEepromVariable
{
public:
    FLProgEepromIntVariable(int16_t startValue = 0);
    int16_t getValue() { return word(value[0], value[1]); };
    void setValue(int16_t newVal);
    virtual uint8_t dataSize() { return 2; };
    virtual uint8_t *getBytes() { return value; };

protected:
    void privateSetValue(int16_t newVal);
    uint8_t value[2];
};

class FLProgEeprom4BytesVariable : public FLProgEepromVariable
{
public:
    virtual uint8_t *getBytes() { return value; };

protected:
    uint8_t value[4];
};

class FLProgEepromLongVariable : public FLProgEeprom4BytesVariable
{
public:
    FLProgEepromLongVariable(long startValue = 0);
    long getValue();
    void setValue(long newVal);

protected:
    void privateSetValue(long newVal);
};

class FLProgEepromUnLongVariable : public FLProgEeprom4BytesVariable
{
public:
    FLProgEepromUnLongVariable(unsigned long startValue = 0);
    unsigned long getValue();
    void setValue(unsigned long newVal);

protected:
    void privateSetValue(unsigned long newVal);
};

class FLProgEepromFloatVariable : public FLProgEeprom4BytesVariable
{
public:
    FLProgEepromFloatVariable(float startValue = 0);
    void setValue(float newVal);
    float getValue();

protected:
    void privateSetValue(float newVal);
};

class FLProgEepromBoolVariable : public FLProgEepromVariable
{
public:
    FLProgEepromBoolVariable(bool startValue = false);
    bool getValue() { return value; };
    void setValue(bool newVal);
    void privateSetValue(bool newValue) { value = newValue; };

protected:
    bool value;
};

class FLProgEepromBoolCollector : public FLProgEepromVariable
{
public:
    FLProgEepromBoolCollector(FLProgEepromBoolVariable *varsTable[], uint16_t tableSyze);
    virtual void setIsNeedWrite(bool value);
    virtual void setByte(uint8_t index, uint8_t value);
    virtual bool getIsNeededWrite();
    virtual uint8_t getWriteByte();
    virtual uint16_t getWriteAddress();
    virtual void next();
    virtual uint8_t dataSize() { return bytesSize; };

protected:
    int16_t writeRegIndex();
    void cilcuateWriteRegIndex();
    FLProgEepromBoolVariable **vars;
    uint16_t varSize;
    uint8_t bytesSize = 1;
    int16_t currentWriteByte = -1;
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
    bool isError() { return codeError; };

    virtual void emergencyRecording(){};

protected:
    void readToVariable(FLProgEepromVariable *var);
    FLProgEepromVariable *nextWriteVar();
    virtual void writeNeededVars(){};
    virtual uint8_t readByte(uint16_t addr) { return 0; };
    virtual uint8_t writeByte(uint16_t addr, uint8_t data){};

    FLProgEepromVariable **vars;
    uint16_t varSize;
    uint8_t codeError = FLPROG_SENSOR_NOT_ERROR;
};

#include "flprog_EEPROM_I2C.h"