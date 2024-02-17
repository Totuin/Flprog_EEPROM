#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include "flprogEEPROM.h"

#ifdef FLPROG_ESP_EEPROM
#include "variant\esp\flprogEspInternalEEPROM.h"
#endif

#ifdef FLPROG_RP2040_EEPROM
#include "variant\rp2040\flprogRP2040InternalEEPROM.h"
#endif

#ifdef FLPROG_STM32_EEPROM
#include "variant\stm32\flprogStm32InternalEEPROM.h"
#endif

#ifdef FLPROG_AVR_EEPROM
#include "variant\avr\flprogAvrInternalEEPROM.h"
#endif