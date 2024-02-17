#include "flprogInternalEEPROM.h"

FLProgInternalEEPROM eeprom(21, 123);

void setup() {
  eeprom.setBooleanStartValue(1, 0, true);
  eeprom.setBooleanStartValue(1, 1, true);
  eeprom.setByteStartValue(2, 1);
  eeprom.setIntegerStartValue(3, 23);
  eeprom.setLongStartValue(5, 25);
  eeprom.setUnsignedLongStartValue(9, 29);
  eeprom.setStringStartValue(12, 10, "1234567890");
  eeprom.begin();
}

void loop() {
  eeprom.pool();

}