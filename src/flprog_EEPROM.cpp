#include "flprog_EEPROM.h"

void FLProgEepromByteVariable::setBytes(uint8_t *data)
{
    value = data[0];
}

void FLProgEepromIntVariable::setBytes(uint8_t *data)
{
    value = word(data[0], data[1]);
}

//--------------------FLProgEeprom4BytesVariable----------

void FLProgEeprom4BytesVariable::setBytes(uint8_t *data)
{

    for (byte i = 0; i < 4; i++)
    {
        value[i] = data[i];
    }
}

//--------------------LONG----------------------
FLProgEepromLongVariable::FLProgEepromLongVariable(long startValue = 0)
{

    uint8_t *x = (uint8_t *)&startValue;
    for (uint8_t i = 0; i < 4; i++)
    {
        value[i] = x[i];
    }
}
long FLProgEepromLongVariable::getValue()
{
    long *y = (long *)&value;
    return y[0];
}

//--------------------Un LONG----------------------
FLProgEepromUnLongVariable::FLProgEepromUnLongVariable(unsigned long startValue)
{

    uint8_t *x = (uint8_t *)&startValue;
    for (uint8_t i = 0; i < 4; i++)
    {
        value[i] = x[i];
    }
}
unsigned long FLProgEepromUnLongVariable::getValue()
{
    unsigned long *y = (unsigned long *)&value;
    return y[0];
}

//--------------------FLOAT----------------------
FLProgEepromFloatVariable::FLProgEepromFloatVariable(float startValue)
{

    byte *x = (byte *)&startValue;
    for (byte i = 0; i < 4; i++)
    {
        value[i] = x[i];
    }
}
float FLProgEepromFloatVariable::getValue()
{
    float *y = (float *)&value;
    return y[0];
}

//------------------------FLProgEepromBaseDevice--------------
void FLProgEepromBaseDevice::setVars(FLProgEepromVariable *varsTable[], uint16_t tableSyze, uint8_t controlIndex)
{
    vars = varsTable;
    varSize = tableSyze;
    uint16_t temp = 1, i;
    for (i = 0; i < tableSyze; i++)
    {
        vars[i]->setAddres(temp);
        temp += vars[i]->dataSize();
    }
    temp = readByte(0);
    if (codeError)
    {
        return;
    }
    if (controlIndex == temp)
    {
        for (i = 0; i < tableSyze; i++)
        {
            readToVariable(vars[i]);
            if (codeError)
            {
                return;
            }
        }
    }
    else
    {
        // emergencyRecording();
    }
}

void FLProgEepromBaseDevice::readToVariable(FLProgEepromVariable *var)
{
    uint8_t data[4];
    readBytes((var->getAddres()), *data, (var->dataSize()));
    if (codeError)
    {
        return;
    }
    var->setBytes(data);
}