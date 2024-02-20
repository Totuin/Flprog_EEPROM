#include "flprogEEPROM.h"

bool FLProgAbstractEEPROM::checkAddres(uint16_t addres, uint16_t endAddres)
{
    if (addres < 1)
    {
        return false;
    }
    if (addres >= _size)
    {
        return false;
    }

    if (endAddres < 1)
    {
        return false;
    }
    if (endAddres >= _size)
    {
        return false;
    }

    return true;
}

uint16_t FLProgAbstractEEPROM::nextUpdateByteAddress()
{
    if (_size == 0)
    {
        return 0;
    }
    for (uint16_t i = 1; i < _size; i++)
    {
        if (_dataChanged[i])
        {
            return i;
        }
    }
    return _size;
}

void FLProgAbstractEEPROM::saveBoolean(uint16_t startAddres, uint8_t bit, bool value, bool needUpdate)
{
    if (!checkAddres(startAddres, startAddres))
    {
        return;
    }
    if (bit > 7)
    {
        return;
    }
    uint8_t temp = _data[startAddres];
    if (bitRead(temp, bit) == value)
    {
        return;
    }
    bitWrite(temp, bit, value);
    _data[startAddres] = temp;
    if (needUpdate)
    {
        _dataChanged[startAddres] = true;
    }
}

void FLProgAbstractEEPROM::saveByte(uint16_t startAddres, uint8_t value, bool needUpdate)
{
    if (!checkAddres(startAddres, startAddres))
    {
        return;
    }
    if (_data[startAddres] == value)
    {
        return;
    }
    _data[startAddres] = value;
    if (needUpdate)
    {
        _dataChanged[startAddres] = true;
    }
}
void FLProgAbstractEEPROM::saveInteger(uint16_t startAddres, int16_t value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + 1)))
    {
        return;
    }
    if (_data[startAddres] != highByte(value))
    {
        _data[startAddres] = highByte(value);
        if (needUpdate)
        {
            _dataChanged[startAddres] = true;
        }
    }
    if (_data[startAddres + 1] != lowByte(value))
    {
        _data[startAddres + 1] = lowByte(value);
        if (needUpdate)
        {
            _dataChanged[startAddres + 1] = true;
        }
    }
}

void FLProgAbstractEEPROM::saveLong(uint16_t startAddres, int32_t value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return;
    }
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    writeForByte(sourse, startAddres, needUpdate);
}

void FLProgAbstractEEPROM::saveFloat(uint16_t startAddres, float value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return;
    }
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    writeForByte(sourse, startAddres, needUpdate);
}

void FLProgAbstractEEPROM::saveUnsignedLong(uint16_t startAddres, uint32_t value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return;
    }
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    writeForByte(sourse, startAddres, needUpdate);
}

void FLProgAbstractEEPROM::saveString(uint16_t startAddres, uint16_t length, String value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + length)))
    {
        return;
    }
    for (uint16_t i = 0; i < length; i++)
    {
        if (i < value.length())
        {
            if (_data[startAddres + i] != (uint8_t)value.charAt(i))
            {
                _data[startAddres + i] = (uint8_t)value.charAt(i);
                if (needUpdate)
                {
                    _dataChanged[startAddres + i] = true;
                }
            }
        }
        else
        {
            if (_data[startAddres + i] != 0)
            {
                _data[startAddres + i] = 0;
                if (needUpdate)
                {
                    _dataChanged[startAddres + i] = true;
                }
            }
        }
    }
}

void FLProgAbstractEEPROM::saveByteArray(uint16_t startAddres, uint16_t length, uint8_t *value, bool needUpdate)
{

    if (!checkAddres(startAddres, (startAddres + length)))
    {
        return;
    }
    for (uint16_t i = 0; i < length; i++)
    {
        if (_data[startAddres + i] != value[i])
        {
            _data[startAddres + i] = value[i];
            if (needUpdate)
            {
                _dataChanged[startAddres + i] = true;
            }
        }
    }
}

void FLProgAbstractEEPROM::readForByte(uint8_t *sourse, uint16_t startAddres)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        sourse[i] = _data[startAddres + i];
    }
}

void FLProgAbstractEEPROM::writeForByte(uint8_t *sourse, uint16_t startAddres, bool needUpdate)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (_data[startAddres + i] != sourse[i])
        {
            _data[startAddres + i] = sourse[i];
            if (needUpdate)
            {
                _dataChanged[startAddres + i] = true;
            }
        }
    }
}

bool FLProgAbstractEEPROM::readBoolean(uint16_t startAddres, uint8_t bit)
{
    if (!checkAddres(startAddres, startAddres))
    {
        return false;
    }
    if (bit > 7)
    {
        return false;
    }
    return bitRead(_data[startAddres], bit);
}

uint8_t FLProgAbstractEEPROM::readByte(uint16_t startAddres)
{
    if (!checkAddres(startAddres, startAddres))
    {
        return 0;
    }
    return _data[startAddres];
}

int16_t FLProgAbstractEEPROM::readInteger(uint16_t startAddres)
{
    if (!checkAddres(startAddres, (startAddres + 1)))
    {
        return 0;
    }
    return word((_data[startAddres]), (_data[startAddres + 1]));
}

int32_t FLProgAbstractEEPROM::readLong(uint16_t startAddres)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return 0;
    }
    int32_t result;
    uint8_t sourse[4];
    readForByte(sourse, startAddres);
    memcpy(&result, sourse, 4);
    return result;
}

float FLProgAbstractEEPROM::readFloat(uint16_t startAddres)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return 0;
    }
    float result;
    uint8_t sourse[4];
    readForByte(sourse, startAddres);
    memcpy(&result, sourse, 4);
    return result;
}

uint32_t FLProgAbstractEEPROM::readUnsignedLong(uint16_t startAddres)
{
    if (!checkAddres(startAddres, (startAddres + 4)))
    {
        return 0;
    }
    uint32_t result;
    uint8_t sourse[4];
    readForByte(sourse, startAddres);
    memcpy(&result, sourse, 4);
    return result;
}

String FLProgAbstractEEPROM::readString(uint16_t startAddres, uint16_t length)
{
    if (!checkAddres(startAddres, (startAddres + length)))
    {
        return "";
    }
    String result = "";
    for (int i = 0; i < length; i++)
    {
        if (_data[startAddres + i] == 0)
        {
            return result;
        }
        result.concat((char)_data[startAddres + i]);
    }
    return result;
}

void FLProgAbstractEEPROM::readByteArray(uint16_t startAddres, uint16_t length, uint8_t *value)
{
    if (!checkAddres(startAddres, (startAddres + length)))
    {
        return;
    }
    for (int i = 0; i < length; i++)
    {
        value[i] = _data[startAddres + i];
    }
}