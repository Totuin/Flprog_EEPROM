#include "flprogAvrInternalEEPROM.h"

#ifdef FLPROG_AVR_EEPROM

FLProgInternalEEPROM::FLProgInternalEEPROM(uint16_t size, uint8_t initByte)
{
    _size = size;
    if (_size > 0)
    {
        _data = new uint8_t[_size];
        _data[0] = initByte;
        _dataChanged = new bool[_size];
    }
}

void FLProgInternalEEPROM::begin()
{
    if (_size == 0)
    {
        return;
    }
    if (EEPROM.read(0) == _data[0])
    {
        for (uint16_t i = 1; i < _size; i++)
        {
            _data[i] = EEPROM.read(i);
            _dataChanged[i] = false;
        }
    }
    else
    {
        EEPROM.write(0, _data[0]);
        for (uint16_t i = 0; i < _size; i++)
        {
            _dataChanged[i] = true;
        }
    }
}

void FLProgInternalEEPROM::pool()
{
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;
    uint16_t temp = nextUpdateByteAddress();
    if (temp == _size)
    {
        return;
    }
    EEPROM.write(temp, _data[temp]);
    _dataChanged[temp] = false;
}
#endif