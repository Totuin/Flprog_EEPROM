#include "flprogEEPROM.h"
#include <string.h>

// sizeof(float)==4 на поддерживаемых Arduino. Формат EEPROM для float остаётся
// побайтовым (как в версии 1.0.0), без изменения порядка байтов.
static_assert(sizeof(float) == 4, "FLProg EEPROM requires 32-bit float");

FLProgAbstractEEPROM::~FLProgAbstractEEPROM()
{
  delete[] _data;
  delete[] _dataChanged;
}

bool FLProgAbstractEEPROM::checkAddres(uint16_t addres, uint16_t endAddres)
{
  // Ячейка 0 принадлежит маркеру. Последний допустимый адрес: _size - 1.
  return _data != nullptr && _dataChanged != nullptr &&
         addres >= 1 && endAddres >= addres && endAddres < _size;
}

bool FLProgAbstractEEPROM::checkRange(uint16_t startAddress, uint32_t byteCount)
{
  if (byteCount == 0 || startAddress == 0 || _size == 0)
  {
    return false;
  }
  const uint32_t last = static_cast<uint32_t>(startAddress) + byteCount - 1u;
  if (last >= _size || last > 0xFFFFu)
  {
    return false;
  }
  return checkAddres(startAddress, static_cast<uint16_t>(last));
}

// По одному изменённому байту за вызов pool() (или больше за счёт
// многократных вызовов) — снижает задержку основного цикла.
uint16_t FLProgAbstractEEPROM::nextUpdateByteAddress()
{
  if (_size == 0 || _dataChanged == nullptr)
  {
    return _size;
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
  if (!checkRange(startAddres, 1))
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
  if (!checkRange(startAddres, 1))
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

void FLProgAbstractEEPROM::saveSignedByte(uint16_t startAddres, int8_t value, bool needUpdate)
{
  if (!checkRange(startAddres, 1))
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

// 16-битное число записывается старшим байтом вперёд.
void FLProgAbstractEEPROM::saveInteger(uint16_t startAddres, int16_t value, bool needUpdate)
{
  if (!checkRange(startAddres, 2))
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

void FLProgAbstractEEPROM::saveUnsignedInteger(uint16_t startAddres, uint16_t value, bool needUpdate)
{

  if (!checkRange(startAddres, 2))
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

// 32-битные значения сохраняются в исходном порядке байтов MCU.
// Это намеренно сохраняет совместимость с существующей EEPROM.
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

// Строка занимает ровно length байтов: хвост заполняется нулями.
// UTF-8 рассматривается как последовательность байтов, не символов.
void FLProgAbstractEEPROM::saveString(uint16_t startAddres, uint16_t length, String value, bool needUpdate)
{
  if (!checkRange(startAddres, length))
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

  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, length))
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
  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, static_cast<uint32_t>(length) * 2u))
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
  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, static_cast<uint32_t>(length) * 4u))
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
  if (!checkRange(startAddres, 1))
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
  if (!checkRange(startAddres, 1))
  {
    return 0;
  }
  return _data[startAddres];
}

int8_t FLProgAbstractEEPROM::readSignedByte(uint16_t startAddres)
{
  if (!checkRange(startAddres, 1))
  {
    return 0;
  }
  return _data[startAddres];
}

int16_t FLProgAbstractEEPROM::readInteger(uint16_t startAddres)
{
  if (!checkRange(startAddres, 2))
  {
    return 0;
  }
  return (int16_t)word((_data[startAddres]), (_data[startAddres + 1]));
}

uint16_t FLProgAbstractEEPROM::readIUnsignednteger(uint16_t startAddres)
{
  if (!checkRange(startAddres, 2))
  {
    return 0;
  }
  return (uint16_t)word((_data[startAddres]), (_data[startAddres + 1]));
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
  if (!checkRange(startAddres, length))
  {
    return "";
  }
  String result = "";
  for (uint16_t i = 0; i < length; i++)
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
  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, length))
  {
    return;
  }
  for (uint16_t i = 0; i < length; i++)
  {
    value[i] = _data[startAddres + i];
  }
}

void FLProgAbstractEEPROM::readUint16Array(uint16_t startAddres, uint16_t length, uint16_t *value)
{
  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, static_cast<uint32_t>(length) * 2u))
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
  if (value == nullptr)
  {
    return;
  }
  if (!checkRange(startAddres, static_cast<uint32_t>(length) * 4u))
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