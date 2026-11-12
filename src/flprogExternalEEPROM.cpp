#include "flprogExternalEEPROM.h"

FLProgExternalEEPROM::FLProgExternalEEPROM(uint8_t bus, uint8_t addres, uint16_t size, uint8_t initByte, uint8_t type)
{
  // Нулевой размер не выделяет буфер и не допускает обращений к _data[0].
  _type = type;
  // Режим 24C0X адресует байт только 8-битным смещением. При размере
  // больше 256 возникало бы переполнение и перезапись начальных ячеек.
  _size = (type == FLPROG_24C0X_EXTERNAL_EEPROM && size > 256) ? 256 : size;
  _writeTime = 0;
  if (_size > 0)
  {
    RT_HW_Base.i2cSetParam(_device, addres, bus, 0);
    _data = new uint8_t[_size];
    _dataChanged = new bool[_size];
    for (uint16_t i = 0; i < _size; i++)
    {
      _data[i] = 0;
      _dataChanged[i] = false;
    }
    _data[0] = initByte;
  }
}

void FLProgExternalEEPROM::begin()
{
  if (_size == 0)
  {
    return;
  }
  RT_HW_Base.i2cInitDevice(_device);
  if (_device.link != 1)
  {
    return;
  }
  const uint8_t marker = universalReadByte(0);
  // Ошибка чтения маркера не должна инициировать полную перезапись чипа.
  if (_device.codeErr != 0)
  {
    return;
  }
  if (marker == _data[0])
  {
    for (uint16_t i = 1; i < _size; i++)
    {
      const uint8_t value = universalReadByte(i);
      if (_device.codeErr != 0)
      {
        // В случае ошибки чтения оставляем уже считанные байты;
        // повторную инициализацию вызывающий код должен выполнить после
        // восстановления связи с устройством.
        return;
      }
      _data[i] = value;
      _dataChanged[i] = false;
    }
  }
  else
  {
    // При другом маркере выполняем первичную инициализацию буфера.
    // Это разрушительная операция — только если маркер успешно прочитан.
    for (uint16_t i = 0; i < _size; i++)
    {
      universalWriteByte(i, _data[i]);
      if (_device.codeErr != 0)
      {
        // Не считаем неуспешную первичную запись завершённой.
        // Для повторной попытки следует вызвать begin() после восстановления I²C.
        return;
      }
      delay(10);
    }
  }
  _writeTime = flprog::timeBack(10);
}

void FLProgExternalEEPROM::pool()
{
  if (_size == 0)
  {
    return;
  }
  if (_device.link != 1)
  {
    begin();
    return;
  }
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
  // RT_HW хранит результат операции в codeErr; не теряем запись,
  // если I²C-транзакция не удалась (повторим на следующем pool()).
  if (_device.codeErr == 0)
  {
    _dataChanged[temp] = false;
  }
}

// Универсальные I2C-операции. Различается длина адреса памяти:
// 24C0X: один байт, 24C2X: два байта (старший, младший).
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
