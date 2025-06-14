#pragma once
#include "Arduino.h"
#include "../../flprogEEPROM.h"

#ifdef FLPROG_STM32_EEPROM
#include <EEPROM.h>
class FLProgInternalEEPROM : public FLProgAbstractEEPROM
{
public:
    FLProgInternalEEPROM(uint16_t size, uint8_t initByte);
    virtual void begin();
    virtual void pool();

protected:
    virtual bool checkAddres(uint16_t addres, uint16_t endAddres);
    bool _hasUpdatedBytes = false;
};
#endif