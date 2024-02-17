#include "flprogEEPROM.h"

FLProgAbstractEEPROM::FLProgAbstractEEPROM(uint16_t size, uint8_t initByte)
{
    this->_size = size;
    if (_size > 0)
    {
        this->_data = new uint8_t[_size];
        this->_data[0] = initByte;
        this->_dataChanged = new bool[_size];
    }
}

bool FLProgAbstractEEPROM::checkAddres(uint16_t addres)
{
    if (addres < 1)
    {
        return false;
    }
    if (addres >= _size)
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
        if (_dataChanged)
        {
            return i;
        }
    }
    return _size;
}

void FLProgAbstractEEPROM::saveBoolean(uint16_t startAddres, uint8_t bit, bool value, bool needUpdate)
{
    if (!checkAddres(startAddres))
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
    if (!checkAddres(startAddres))
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
    if (!checkAddres(startAddres))
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
    if (!checkAddres(startAddres))
    {
        return;
    }
    uint8_t *x = (uint8_t *)&value;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (_data[startAddres + i] != x[i])
        {
            _data[startAddres + i] = x[i];
            if (needUpdate)
            {
                _dataChanged[startAddres + i] = true;
            }
        }
    }
}

void FLProgAbstractEEPROM::saveUnsignedLong(uint16_t startAddres, uint32_t value, bool needUpdate)
{
    if (!checkAddres(startAddres))
    {
        return;
    }
    uint8_t *x = (uint8_t *)&value;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (_data[startAddres + i] != x[i])
        {
            _data[startAddres + i] = x[i];
            if (needUpdate)
            {
                _dataChanged[startAddres + i] = true;
            }
        }
    }
}

void FLProgAbstractEEPROM::saveString(uint16_t startAddres, uint16_t length, String value, bool needUpdate)
{
    if (!checkAddres(startAddres))
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

bool FLProgAbstractEEPROM::readBoolean(uint16_t startAddres, uint8_t bit)
{
    if (!checkAddres(startAddres))
    {
        return false;
    }
    return bitRead(_data[startAddres], bit);
}

uint8_t FLProgAbstractEEPROM::readByte(uint16_t startAddres, uint8_t value)
{
    if (!checkAddres(startAddres))
    {
        return 0;
    }
    return _data[startAddres];
}

int16_t FLProgAbstractEEPROM::readInteger(uint16_t startAddres)
{
    if (!checkAddres(startAddres))
    {
        return 0;
    }
    return word((_data[startAddres]), (_data[startAddres + 1]));
}

int32_t FLProgAbstractEEPROM::readLong(uint16_t startAddres)
{

    uint8_t x[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        x[i] = _data[startAddres + i];
    }
    int32_t *y = (long *)&x;
    return y[0];
}

uint32_t FLProgAbstractEEPROM::readUnsignedLong(uint16_t startAddres)
{
    uint8_t x[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        x[i] = _data[startAddres + i];
    }
    uint32_t *y = (unsigned long *)&x;
    return y[0];
}

String FLProgAbstractEEPROM::readString(uint16_t startAddres, uint16_t length)
{
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
