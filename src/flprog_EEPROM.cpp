#include "flprog_EEPROM.h"

void FLProgEepromVariable::next()
{
    pointer++;
    if (!(pointer < dataSize()))
    {
        pointer = 0;
        isNeededWrite = false;
    }
}

//------------------------BYTE-------------------------------
void FLProgEepromByteVariable::setValue(uint8_t newVal)
{
    if (newVal == getValue())
    {
        return;
    }
    value[0] = newVal;
    pointer = 0;
    isNeededWrite = true;
}

//-------------------------------INTEGER--------------------

FLProgEepromIntVariable::FLProgEepromIntVariable(int16_t startValue)
{
    privateSetValue(startValue);
}

void FLProgEepromIntVariable::privateSetValue(int16_t newVal)
{
    value[0] = highByte(newVal);
    value[1] = lowByte(newVal);
}

void FLProgEepromIntVariable::setValue(int16_t newVal)
{
    if (newVal == getValue())
    {
        return;
    }
    privateSetValue(newVal);
    pointer = 0;
    isNeededWrite = true;
}

//--------------------FLProgEeprom4BytesVariable----------

//--------------------LONG----------------------
FLProgEepromLongVariable::FLProgEepromLongVariable(long startValue)
{
    privateSetValue(startValue);
}
long FLProgEepromLongVariable::getValue()
{
    return long(*((long *)&value));
}

void FLProgEepromLongVariable::setValue(long newVal)
{
    if (newVal == getValue())
    {
        return;
    }
    privateSetValue(newVal);
    pointer = 0;
    isNeededWrite = true;
}

void FLProgEepromLongVariable::privateSetValue(long newVal)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        value[i] = *(((byte *)&newVal) + i);
    }
}

//--------------------Un LONG----------------------
FLProgEepromUnLongVariable::FLProgEepromUnLongVariable(unsigned long startValue)
{
    privateSetValue(startValue);
}
unsigned long FLProgEepromUnLongVariable::getValue()
{
    return uint32_t(*((unsigned long *)&value));
}

void FLProgEepromUnLongVariable::setValue(unsigned long newVal)
{
    if (newVal == getValue())
    {
        return;
    }
    privateSetValue(newVal);
    pointer = 0;
    isNeededWrite = true;
}

void FLProgEepromUnLongVariable::privateSetValue(unsigned long newVal)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        value[i] = *(((byte *)&newVal) + i);
    }
}

//--------------------FLOAT----------------------
FLProgEepromFloatVariable::FLProgEepromFloatVariable(float startValue)
{
    privateSetValue(startValue);
}
float FLProgEepromFloatVariable::getValue()
{
    return float(*((float *)&value));
}

void FLProgEepromFloatVariable::setValue(float newVal)
{
    if (newVal == getValue())
    {
        return;
    }
    privateSetValue(newVal);
    pointer = 0;
    isNeededWrite = true;
}

void FLProgEepromFloatVariable::privateSetValue(float newVal)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        value[i] = *(((byte *)&newVal) + i);
    }
}

//---------------------------------Bool-----------------------------
FLProgEepromBoolVariable::FLProgEepromBoolVariable(bool startValue)
{
    value = startValue;
}

void FLProgEepromBoolVariable::setValue(bool newVal)
{
    if (newVal == value)
    {
        return;
    }
    value = newVal;
    isNeededWrite = true;
}

// ---------------------BoolCollector------------------------

FLProgEepromBoolCollector::FLProgEepromBoolCollector(FLProgEepromBoolVariable *varsTable[], uint16_t tableSyze)
{
    vars = varsTable;
    varSize = tableSyze;
    if (tableSyze > 8)
    {
        bytesSize = tableSyze / 8;
        if ((bytesSize * 8) < tableSyze)
        {
            bytesSize++;
        }
    }
}

void FLProgEepromBoolCollector::setIsNeedWrite(bool value)
{
    for (uint16_t i = 0; i < varSize; i++)
    {
        vars[i]->setIsNeedWrite(value);
    }
}

void FLProgEepromBoolCollector::setByte(uint8_t index, uint8_t value)
{
    uint16_t startAddress = index * 8;
    if (!(startAddress < varSize))
    {
        return;
    }
    for (uint8_t i = 0; i < +8; i++)
    {
        if (!((startAddress + i) < varSize))
        {
            return;
        }
        vars[startAddress + i]->privateSetValue(bitRead(value, i));
    }
}

bool FLProgEepromBoolCollector::getIsNeededWrite()
{
    return !(writeRegIndex() < 0);
}

int16_t FLProgEepromBoolCollector::writeRegIndex()
{
    // if (!(currentWriteByte < 0))
    // {
    cilcuateWriteRegIndex();
    return currentWriteByte;

    //  }
    // return currentWriteByte;
}

void FLProgEepromBoolCollector::cilcuateWriteRegIndex()
{
    int16_t byte = 0;
    uint8_t currentBit = 0;
    for (uint16_t i = 0; i < varSize; i++)
    {
        if (vars[i]->getIsNeededWrite())
        {
            currentWriteByte = byte;
            return;
        }
        currentBit++;
        if (currentBit > 7)
        {
            currentBit = 0;
            byte++;
        }
    }
    currentWriteByte = -1;
}

uint8_t FLProgEepromBoolCollector::getWriteByte()
{
    uint8_t result = 0;
    uint16_t varAddres = (writeRegIndex()) * 8;
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((varAddres + i) < varSize)
        {
            bitWrite(result, i, (vars[i]->getValue()));
        }
        else
        {
            return result;
        }
    }
    return result;
}

uint16_t FLProgEepromBoolCollector::getWriteAddress()
{
    return writeRegIndex() + addres;
}

void FLProgEepromBoolCollector::next()
{
    uint8_t result = 0;
    currentWriteByte = -1;
    uint16_t varAddres = (writeRegIndex()) * 8;
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((varAddres + i) < varSize)
        {
            vars[i]->setIsNeedWrite(false);
        }
        else
        {
            return;
        }
    }
}
//--------------------------String---------------------------------
FLProgEepromStringVariable::FLProgEepromStringVariable(uint16_t maxSize, String startValue)
{
    size = maxSize + 1;
    free(value);
    value = (char *)malloc(size);
    privateSetValue(startValue);
}

void FLProgEepromStringVariable::privateSetValue(String newValue)
{
    newValue.toCharArray(value, size);
}

void FLProgEepromStringVariable::setValue(String newVal)
{
    if (newVal.equals(value))
    {
        return;
    }
    privateSetValue(newVal);
    pointer = 0;
    isNeededWrite = true;
}

uint8_t FLProgEepromStringVariable::getWriteByte()
{

    return (uint8_t)value[pointer];
}

void FLProgEepromStringVariable::setByte(uint8_t index, uint8_t newValue)
{
    value[index] = (char)newValue;
}

//------------------------FLProgEepromBaseDevice--------------------
void FLProgEepromBaseDevice::setVars(FLProgEepromVariable *varsTable[], uint16_t tableSyze, uint8_t controlIndex)
{
    vars = varsTable;
    varSize = tableSyze;
    uint16_t temp = 1, i;
    uint8_t index = readByte(0);
    bool isfirstRun = controlIndex != index;
    if (codeError)
    {
        return;
    }
    for (i = 0; i < tableSyze; i++)
    {
        vars[i]->setAddres(temp);
        vars[i]->setIsNeedWrite(isfirstRun);
        temp += vars[i]->dataSize();
    }
    if (isfirstRun)
    {
        writeByte(0, controlIndex);
        if (codeError)
        {
            return;
        }
        delay(FLPROG_EEPROM_I2C_AFTER_WRITE_DELAY);
        emergencyRecording();
    }
    else
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
}

void FLProgEepromBaseDevice::readToVariable(FLProgEepromVariable *var)
{
    uint8_t result;
    for (uint8_t i = 0; i < var->dataSize(); i++)
    {
        result = readByte((var->getAddres()) + i);
        if (codeError)
        {
            return;
        }
        var->setByte(i, result);
    }
}

FLProgEepromVariable *FLProgEepromBaseDevice::nextWriteVar()
{
    for (uint16_t i = 0; i < varSize; i++)
    {
        if (vars[i]->getIsNeededWrite())
        {
            return vars[i];
        }
    }
    return 0;
}
