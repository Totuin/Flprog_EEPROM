#include "flprog_EEPROM.h"

FLProgEepromByteVariable byteVar1(10);
FLProgEepromByteVariable byteVar2;

FLProgEepromIntVariable intVar1(11);
FLProgEepromIntVariable intVar2;

FLProgEepromLongVariable longVar1(12);
FLProgEepromLongVariable longVar2;

FLProgEepromUnLongVariable unLongVar1(13);
FLProgEepromUnLongVariable unLongVar2;

FLProgEepromFloatVariable floatVar1(14.3);
FLProgEepromFloatVariable floatVar2;

FLProgEepromVariable *varsInEEprom[10] = {&byteVar1, &byteVar2, &intVar1, &intVar2, &longVar1, &longVar2, &unLongVar1, &unLongVar2, &floatVar1, &floatVar2};
FLProgI2C wireDevice(0);
FLProg24C2X Eeprom1(&wireDevice);

uint32_t startTime;

void setup()
{
    Serial.begin(9600);
    wireDevice.begin();
    Eeprom1.setVars(varsInEEprom, 10, 3);

    startTime = flprog::timeBack(1000);
}
void loop()
{
    if (flprog::isTimer(startTime, 1000))
    {
        Serial.print("Byte1 - ");
        Serial.println(byteVar1.getValue());
        Serial.print("Byte2 - ");
        Serial.println(byteVar2.getValue());
        Serial.print("Int1 - ");
        Serial.println(intVar1.getValue());
        Serial.print("Int2 - ");
        Serial.println(intVar2.getValue());
        Serial.print("Long1 - ");
        Serial.println(longVar1.getValue());
        Serial.print("Long2 - ");
        Serial.println(longVar2.getValue());
        Serial.print("Un Long1 - ");
        Serial.println(unLongVar1.getValue());
        Serial.print("Un Long2 - ");
        Serial.println(unLongVar2.getValue());
        Serial.print("Float1 - ");
        Serial.println(floatVar1.getValue());
        Serial.print("Float2 - ");
        Serial.println(floatVar2.getValue());
        startTime = millis();
    }
}