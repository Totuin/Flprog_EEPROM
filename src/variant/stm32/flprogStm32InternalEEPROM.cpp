#include "flprogStm32InternalEEPROM.h"

#ifdef FLPROG_STM32_EEPROM

bool FLProgInternalEEPROM::checkAddres(uint16_t addres)
{
    if (addres < 1)
    {
        return false;
    }
    if (addres >= EEPROM.length())
    {
        return false;
    }
    if (addres >= _size)
    {
        return false;
    }
    return true;
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
        for (uint16_t i = 0; i < _size; i++)
        {
            EEPROM.write(i, _data[i]);
            _dataChanged[i] = false;
        }
    }
}

void FLProgInternalEEPROM::pool()
{
    uint16_t temp = nextUpdateByteAddress();
    if (temp == _size)
    {
        return;
    }
    EEPROM.write(temp, _data[temp]);
    _dataChanged[temp] = false;
}
#endif