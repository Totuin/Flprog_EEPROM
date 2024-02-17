#pragma once
#include "Arduino.h"
#include "../../flprogEEPROM.h"
#include <EEPROM.h>
#ifdef FLPROG_RP2040_EEPROM
class FLProgInternalEEPROM : public FLProgAbstractEEPROM
{
public:
    FLProgInternalEEPROM(uint16_t size, uint8_t initByte) : FLProgAbstractEEPROM(size, initByte){};
    virtual void begin();
    virtual void pool();

protected:
    bool _isNeedCommit = false;
};
#endif