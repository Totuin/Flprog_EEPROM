#include "flprogExternalEEPROM.h"

FLProgExternalEEPROM eeprom(0, 0x50, 30, 6);

bool IsWriter = false;
void setup()
{
  Serial.begin(115200);
  eeprom.setBooleanStartValue(1, 0, true);
  eeprom.setByteStartValue(2, 1);
  eeprom.setIntegerStartValue(3, 23);
  eeprom.setLongStartValue(5, 25);
  eeprom.setUnsignedLongStartValue(9, 29);
  eeprom.setStringStartValue(13, 10, "A");
  eeprom.begin();
}

void loop()
{
  eeprom.pool();
  if (!IsWriter)
  { Serial.println("");
    Serial.print("Status - ");
    Serial.println(eeprom.device()->status);
    Serial.print("Error - ");
    Serial.println(eeprom.device()->codeErr);
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
    Serial.println((unsigned long)eeprom.readUnsignedLong(9));
    Serial.print("String - ");
    String temp = eeprom.readString(13, 10);
    Serial.println(temp);
    Serial.println("Write..............");
    eeprom.saveBoolean(1, 0, (!eeprom.readBoolean(1, 0)));
    eeprom.saveByte(2, ((eeprom.readByte(2)) + 1));
    eeprom.saveInteger(3, (eeprom.readInteger(3) + 1));
    eeprom.saveLong(5, (eeprom.readLong(5) + 1));
    eeprom.saveUnsignedLong(9, (eeprom.readUnsignedLong(9) + 1));
    if (temp.length() >= 9)
    {
      temp = "";
    }
    temp.concat("A");
    eeprom.saveString(13, 10, temp);
    Serial.print("Bool - ");
    Serial.println(eeprom.readBoolean(1, 0));
    Serial.print("Byte - ");
    Serial.println(eeprom.readByte(2));
    Serial.print("Integer - ");
    Serial.println(eeprom.readInteger(3));
    Serial.print("Long - ");
    Serial.println(eeprom.readLong(5));
    Serial.print("Unsigned Long - ");
    Serial.println((unsigned long)eeprom.readUnsignedLong(9));
    Serial.print("String - ");
    temp = eeprom.readString(13, 10);
    Serial.println(temp);
    IsWriter = true;
  }
}