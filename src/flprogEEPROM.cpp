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
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    saveByteArray(startAddres, 4, sourse, needUpdate);
}

void FLProgAbstractEEPROM::saveFloat(uint16_t startAddres, float value, bool needUpdate)
{
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    saveByteArray(startAddres, 4, sourse, needUpdate);
}

void FLProgAbstractEEPROM::saveUnsignedLong(uint16_t startAddres, uint32_t value, bool needUpdate)
{
    uint8_t sourse[4];
    memcpy(sourse, &value, 4);
    saveByteArray(startAddres, 4, sourse, needUpdate);
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

void FLProgAbstractEEPROM::saveUint16Array(uint16_t startAddres, uint16_t length, uint16_t *value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + (length * 2))))
    {
        return;
    }
    uint16_t dataAddres = startAddres;
    uint16_t valTemp;
    uint8_t sourse[2];
    for (uint16_t i = 0; i < length; i++)
    {
        valTemp = value[i];
        memcpy(sourse, &valTemp, 2);
        saveByteArray(dataAddres, 2, sourse, needUpdate);
        dataAddres = dataAddres + 2;
    }
}

void FLProgAbstractEEPROM::saveUint32Array(uint16_t startAddres, uint16_t length, uint32_t *value, bool needUpdate)
{
    if (!checkAddres(startAddres, (startAddres + (length * 4))))
    {
        return;
    }
    uint8_t sourse[4];
    uint16_t dataAddres = startAddres;
    uint32_t valTemp;
    for (uint16_t i = 0; i < length; i++)
    {
        valTemp = value[i];
        memcpy(sourse, &valTemp, 4);
        saveByteArray(dataAddres, 4, sourse, needUpdate);
        dataAddres = dataAddres + 4;
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
    int32_t result;
    uint8_t sourse[4] = {0, 0, 0, 0};
    readByteArray(startAddres, 4, sourse);
    memcpy(&result, sourse, 4);
    return result;
}

float FLProgAbstractEEPROM::readFloat(uint16_t startAddres)
{

    float result;
    uint8_t sourse[4] = {0, 0, 0, 0};
    readByteArray(startAddres, 4, sourse);
    memcpy(&result, sourse, 4);
    return result;
}

uint32_t FLProgAbstractEEPROM::readUnsignedLong(uint16_t startAddres)
{
    uint32_t result;
    uint8_t sourse[4] = {0, 0, 0, 0};
    readByteArray(startAddres, 4, sourse);
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

void FLProgAbstractEEPROM::readUint16Array(uint16_t startAddres, uint16_t length, uint16_t *value)
{
    if (!checkAddres(startAddres, (startAddres + (length * 2))))
    {
        return;
    }
    uint16_t dataAddres = startAddres;
    uint8_t sourse[2] = {0, 0};
    uint16_t result;
    for (uint16_t i = 0; i < length; i++)
    {
        readByteArray(dataAddres, 2, sourse);
        memcpy(&result, sourse, 2);
        value[i] = result;
        dataAddres = dataAddres + 2;
    }
}

void FLProgAbstractEEPROM::readUint32Array(uint16_t startAddres, uint16_t length, uint32_t *value)
{
    if (!checkAddres(startAddres, (startAddres + (length * 4))))
    {
        return;
    }
    uint16_t dataAddres = startAddres;
    uint8_t sourse[4] = {0, 0, 0, 0};
    uint32_t result;
    for (uint16_t i = 0; i < length; i++)
    {
        readByteArray(dataAddres, 4, sourse);
        memcpy(&result, sourse, 4);
        value[i] = result;
        dataAddres = dataAddres + 4;
    }
}