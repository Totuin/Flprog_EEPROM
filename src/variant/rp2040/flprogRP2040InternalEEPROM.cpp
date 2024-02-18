#include "flprogRP2040InternalEEPROM.h"

#ifdef FLPROG_RP2040_EEPROM
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
    EEPROM.begin(_size);
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
        for (uint16_t i = 0; i < _size; i++)
        {
            EEPROM.write(i, _data[i]);
            _dataChanged[i] = false;
        }
        EEPROM.commit();
    }
}

void FLProgInternalEEPROM::pool()
{
    uint16_t temp = nextUpdateByteAddress();
    if (temp != _size)
    {
        EEPROM.write(temp, _data[temp]);
        _dataChanged[temp] = false;
        _isNeedCommit = true;
        return;
    }
    if (_isNeedCommit)
    {
        EEPROM.commit();
        _isNeedCommit = false;
    }
}
#endif