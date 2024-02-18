#include "flprogInternalEEPROM.h"

FLProgInternalEEPROM eeprom(21, 123);

bool IsWriter = false;
void setup() {
  Serial.begin(115200);
  eeprom.setBooleanStartValue(1, 0, true);
  eeprom.setByteStartValue(2, 1);
  eeprom.setIntegerStartValue(3, 23);
  eeprom.setLongStartValue(5, 25);
  eeprom.setUnsignedLongStartValue(9, 29);
  eeprom.setStringStartValue(12, 10, "A");
  eeprom.begin();
}

void loop() {
  eeprom.pool();
  if (! IsWriter)
  {
    Serial.println("Start..............");
    Serial.print("Bool - ");
    Serial.println(eeprom.readBoolean(1, 0));
    Serial.print("Byte - ");
    Serial.println(eeprom.readByte(2));
    Serial.print("Integer - ");
    Serial.println(eeprom.readInteger(3));
    Serial.print("Long - ");
    Serial.println(eeprom.readLong(5));
    Serial.print("Unsigned Long - ");
    Serial.println(eeprom.readUnsignedLong(9));
    Serial.print("String - ");
    String temp = eeprom.readString(12, 10);
    Serial.println(temp);
    Serial.println("Write..............");
    eeprom.saveBoolean(1, 0, (!eeprom.readBoolean(1, 0)));
    eeprom.saveByte(2, ((eeprom.readByte(2)) + 1));
    eeprom.saveInteger(3, (eeprom.readInteger(3) + 1));
    eeprom.saveLong(5, (eeprom.readLong(5) + 1));
    eeprom.saveUnsignedLong(9, (eeprom.readUnsignedLong(9) + 1));
    if (temp.length() >= 9) {
      temp = "";
    }
    temp.concat("A");
    eeprom.saveString(12, 10, temp);
    Serial.print("Bool - ");
    Serial.println(eeprom.readBoolean(1, 0));
    Serial.print("Byte - ");
    Serial.println(eeprom.readByte(2));
    Serial.print("Integer - ");
    Serial.println(eeprom.readInteger(3));
    Serial.print("Long - ");
    Serial.println(eeprom.readLong(5));
    Serial.print("Unsigned Long - ");
    Serial.println(eeprom.readUnsignedLong(9));
    Serial.print("String - ");
    temp = eeprom.readString(12, 10);
    Serial.println(temp);
    IsWriter=true;
  }


}