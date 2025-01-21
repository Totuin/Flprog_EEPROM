#include "flprogExternalEEPROM.h"

FLProgExternalEEPROM::FLProgExternalEEPROM(uint8_t bus, uint8_t addres, uint16_t size, uint8_t initByte, uint8_t type)
{
    _size = size;
    if (_size > 0)
    {
        RT_HW_Base.i2cSetParam(_device, addres, bus);
        _type = type;
        _data = new uint8_t[_size];
        _dataChanged = new bool[_size];
        for (uint16_t i = 0; i < _size; i++)
        {
            _data[i] = 0;
            _dataChanged[i] = false;
        }
    }
    _data[0] = initByte;
}

void FLProgExternalEEPROM::begin()
{
    RT_HW_Base.i2cInitDevice(_device);
    if (universalReadByte(0) == _data[0])
    {
        for (uint16_t i = 1; i < _size; i++)
        {
            _data[i] = universalReadByte(i);
            _dataChanged[i] = false;
        }
    }
    else
    {
        universalWriteByte(0, _data[0]);
        for (uint16_t i = 0; i < _size; i++)
        {
            // universalWriteByte(i, _data[i]);
            // delay(10);
            _dataChanged[i] = true;
        }
    }
    _writeTime = flprog::timeBack(10);
}

void FLProgExternalEEPROM::pool()
{
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;

    if (!(flprog::isTimer(_writeTime, 10)))
    {
        return;
    }
    uint16_t temp = nextUpdateByteAddress();
    if (temp == _size)
    {
        return;
    }
    universalWriteByte(temp, _data[temp]);
    _writeTime = millis();
    _dataChanged[temp] = false;
}

uint8_t FLProgExternalEEPROM::universalReadByte(uint16_t addr)
{
    if (_type == FLPROG_24C0X_EXTERNAL_EEPROM)
    {
        return readByte24C0X(addr);
    }
    uint8_t data[2];
    data[0] = addr >> 8;
    data[1] = addr & 0xFF;
    RT_HW_Base.i2cWriteArr(_device, data, 2);
    RT_HW_Base.i2cRead(_device);
    return _device.bf8;
}

void FLProgExternalEEPROM::universalWriteByte(uint16_t addr, uint8_t value)
{
    if (_type == FLPROG_24C0X_EXTERNAL_EEPROM)
    {
        writeByte24C0X(addr, value);
        return;
    }
    uint8_t data[3];
    data[0] = addr >> 8;
    data[1] = addr & 0xFF;
    data[2] = value;
    RT_HW_Base.i2cWriteArr(_device, data, 3);
}

uint8_t FLProgExternalEEPROM::readByte24C0X(uint16_t addr)
{
    RT_HW_Base.i2cWrite(_device, (uint8_t)addr);
    RT_HW_Base.i2cRead(_device);
    return _device.bf8;
}

void FLProgExternalEEPROM::writeByte24C0X(uint16_t addr, uint8_t value)
{
    uint8_t data[2];
    data[0] = (uint8_t)addr;
    data[1] = value;
    RT_HW_Base.i2cWriteArr(_device, data, 2);
}
