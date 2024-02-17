#include "flprogEspInternalEEPROM.h"

#ifdef FLPROG_ESP_EEPROM
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