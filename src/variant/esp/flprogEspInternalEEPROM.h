#pragma once
#include "Arduino.h"
#include "../../flprogEEPROM.h"

#ifdef FLPROG_ESP_EEPROM
#include <EEPROM.h>
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