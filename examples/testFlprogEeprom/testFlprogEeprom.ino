#include "flprog_EEPROM.h"

FLProgEepromByteVariable byteVar1(152);
FLProgEepromByteVariable byteVar2;

FLProgEepromIntVariable intVar1(3333);
FLProgEepromIntVariable intVar2;

FLProgEepromLongVariable longVar1(4444);
FLProgEepromLongVariable longVar2;

FLProgEepromUnLongVariable unLongVar1(55555);
FLProgEepromUnLongVariable unLongVar2;

FLProgEepromFloatVariable floatVar1(66.66);
FLProgEepromFloatVariable floatVar2;

FLProgEepromBoolVariable boolVar1(true);
FLProgEepromBoolVariable boolVar2;
FLProgEepromBoolVariable boolVar3(true);
FLProgEepromBoolVariable *booVarsInEEprom[10] = {&boolVar1, &boolVar2, &boolVar3};

FLProgEepromBoolCollector collector(booVarsInEEprom, 3);

FLProgEepromVariable *varsInEEprom[11] = {&byteVar1, &byteVar2, &intVar1, &intVar2, &longVar1, &longVar2, &unLongVar1, &unLongVar2, &floatVar1, &floatVar2, &collector };
FLProgI2C wireDevice(0);
FLProg24C2X Eeprom1(&wireDevice);

uint32_t startTime;
bool oldValue = false;


bool _bounseInputD2S = 0;
bool _bounseInputD2O = 0;
unsigned long _bounseInputD2P = 0UL;

void setup()
{
  Serial.begin(9600);
  wireDevice.begin();
  Eeprom1.setVars(varsInEEprom, 10, 8);
  pinMode(2, INPUT_PULLUP);
  startTime = flprog::timeBack(1000);
  _bounseInputD2O =    digitalRead(2);

}
void loop()
{
  Eeprom1.pool();

  bool  _bounceInputTmpD2 =  (digitalRead (2));
  if (_bounseInputD2S)
  {
    if (millis() >= (_bounseInputD2P + 40))
    {
      _bounseInputD2O = _bounceInputTmpD2;
      _bounseInputD2S = 0;
    }
  }
  else
  {
    if (_bounceInputTmpD2 != _bounseInputD2O)
    {
      _bounseInputD2S = 1;
      _bounseInputD2P = millis();
    }
  }







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
    Serial.println();
    Serial.println();
    startTime = millis();
  }

  if ( _bounseInputD2O) {

    if (! oldValue)
    {
      oldValue = true;
      byteVar2.setValue(100);
      intVar2.setValue(101);
      longVar2.setValue(102);
      unLongVar2.setValue(103);
      floatVar2.setValue(104.5);
    }
    else
    {
      oldValue = false;
    }
  }


}