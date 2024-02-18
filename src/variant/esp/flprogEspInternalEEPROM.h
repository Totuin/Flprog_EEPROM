#pragma once
#include "Arduino.h"
#include "../../flprogEEPROM.h"
#include <EEPROM.h>
#ifdef FLPROG_ESP_EEPROM
class FLProgInternalEEPROM : public FLProgAbstractEEPROM
{
public:
    FLProgInternalEEPROM(uint16_t size, uint8_t initByte);
    virtual void begin();
    virtual void pool();

protected:
    bool _isNeedCommit = false;
};
#endif