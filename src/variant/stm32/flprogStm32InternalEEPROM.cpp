#include "flprogStm32InternalEEPROM.h"

#ifdef FLPROG_STM32_EEPROM

// size включает байт-маркер 0; остаток хранит значения пользователя.
FLProgInternalEEPROM::FLProgInternalEEPROM(uint16_t size, uint8_t initByte)
{
    _size = size;
    if (_size > 0)
    {
        _data = new uint8_t[_size];
        _dataChanged = new bool[_size];
        // Незаполненные начальные байты должны быть определены (0),
        // иначе первая инициализация записывает в EEPROM мусор из RAM.
        for (uint16_t i = 0; i < _size; ++i)
        {
            _data[i] = 0;
            _dataChanged[i] = false;
        }
        _data[0] = initByte;
    }
}

bool FLProgInternalEEPROM::checkAddres(uint16_t addres, uint16_t endAddres)
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
    if (endAddres < addres)
    {
        return false;
    }
    if (endAddres < 1)
    {
        return false;
    }
    if (endAddres >= EEPROM.length())
    {
        return false;
    }
    if (endAddres >= _size)
    {
        return false;
    }
    return true;
}

// Если маркер совпадает, восстанавливаем RAM из EEPROM.
// Иначе используем заданные до begin() начальные значения.
void FLProgInternalEEPROM::begin()
{
    if (_size == 0)
    {
        return;
    }
    // Не читаем за физический предел поддерживаемой STM32 EEPROM.
    if (_size > EEPROM.length())
    {
        return;
    }
    // if (EEPROM.read(0) == _data[0])
    if (eeprom_read_byte(0) == _data[0])
    {
        for (uint16_t i = 1; i < _size; i++)
        {
            // _data[i] = EEPROM.read(i);
            _data[i] = eeprom_buffered_read_byte(i);
            _dataChanged[i] = false;
        }
    }
    else
    {
        // EEPROM.write(0, _data[0]);
        for (uint16_t i = 0; i < _size; i++)
        {
            eeprom_buffered_write_byte(i, _data[i]);
            // EEPROM.write(i, _data[i]);
            _dataChanged[i] = true;
        }
        eeprom_buffer_flush();
    }
}

// Отложенная запись одного изменённого байта за проход.
// Для эмулируемой EEPROM на Flash дополнительно требуется commit()/flush().
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
        if (_hasUpdatedBytes)
        {
            eeprom_buffer_flush();
            _hasUpdatedBytes = false;
        }
        return;
    }
    // EEPROM.write(temp, _data[temp]);
    eeprom_buffered_write_byte(temp, _data[temp]);
    _hasUpdatedBytes = true;
    _dataChanged[temp] = false;
}
#endif