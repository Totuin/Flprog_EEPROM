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
FLProgEepromBoolVariable boolVar4;
FLProgEepromBoolVariable boolVar5(true);
FLProgEepromBoolVariable boolVar6;
FLProgEepromBoolVariable boolVar7(true);
FLProgEepromBoolVariable boolVar8;
FLProgEepromBoolVariable boolVar9(true);
FLProgEepromBoolVariable boolVar10;
FLProgEepromBoolVariable *booVarsInEEprom[10] = {&boolVar1, &boolVar2, &boolVar3, &boolVar4, &boolVar5, &boolVar6, &boolVar7, &boolVar8, &boolVar9, &boolVar10};
FLProgEepromBoolCollector collector(booVarsInEEprom, 10);

FLProgEepromStringVariable stringVar1(5, "Start");
FLProgEepromStringVariable stringVar2(6, "Start12");
FLProgEepromStringVariable stringVar3(10);

FLProgEepromStringVariable stringVar4(2, "AB");
FLProgEepromStringVariable stringVar5(2, "CD");

FLProgEepromVariable *varsInEEprom[15] = {&byteVar1, &byteVar2, &intVar1, &intVar2, &longVar1, &longVar2, &unLongVar1, &unLongVar2, &floatVar1, &floatVar2,  &stringVar1 , &stringVar2, &stringVar3, &stringVar4, &stringVar5 };
// FLProgEepromVariable *varsInEEprom[2] = {&stringVar4,&stringVar5  };
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
  Eeprom1.setVars(varsInEEprom, 15, 200);
  pinMode(2, INPUT_PULLUP);
  startTime = flprog::timeBack(1000);
  _bounseInputD2O = digitalRead(2);
  delay(2000);
}
void loop()
{
  Eeprom1.pool();

  bool _bounceInputTmpD2 = (digitalRead(2));
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
    Serial.print(byteVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(byteVar1.getAddres());
    Serial.println();

    Serial.print("Byte2 - ");
    Serial.print(byteVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(byteVar2.getAddres());
    Serial.println();

    Serial.print("Int1 - ");
    Serial.print(intVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(intVar1.getAddres());
    Serial.println();

    Serial.print("Int2 - ");
    Serial.print(intVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(intVar2.getAddres());
    Serial.println();

    Serial.print("Long1 - ");
    Serial.print(longVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(longVar1.getAddres());
    Serial.println();

    Serial.print("Long2 - ");
    Serial.print(longVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(longVar2.getAddres());
    Serial.println();

    Serial.print("Un Long1 - ");
    Serial.print(unLongVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(unLongVar1.getAddres());
    Serial.println();

    Serial.print("Un Long2 - ");
    Serial.print(unLongVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(unLongVar2.getAddres());
    Serial.println();

    Serial.print("Float1 - ");
    Serial.print(floatVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(floatVar1.getAddres());
    Serial.println();

    Serial.print("Float2 - ");
    Serial.print(floatVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(floatVar2.getAddres());
    Serial.println();

    Serial.print("Bool1 - ");
    Serial.print(boolVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar1.getAddres());
    Serial.println();

    Serial.print("Bool2 - ");
    Serial.print(boolVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar2.getAddres());
    Serial.println();

    Serial.print("Bool3 - ");
    Serial.print(boolVar3.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar3.getAddres());
    Serial.println();

    Serial.print("Bool4 - ");
    Serial.print(boolVar4.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar4.getAddres());
    Serial.println();

    Serial.print("Bool5 - ");
    Serial.print(boolVar5.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar5.getAddres());
    Serial.println();

    Serial.print("Bool6 - ");
    Serial.print(boolVar6.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar6.getAddres());
    Serial.println();

    Serial.print("Bool7 - ");
    Serial.print(boolVar7.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar7.getAddres());
    Serial.println();

    Serial.print("Bool8 - ");
    Serial.print(boolVar8.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar8.getAddres());
    Serial.println();

    Serial.print("Bool9 - ");
    Serial.print(boolVar9.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar9.getAddres());
    Serial.println();

    Serial.print("Bool10 - ");
    Serial.print(boolVar10.getValue());
    Serial.print(" Address - ");
    Serial.print(boolVar10.getAddres());
    Serial.println();

    Serial.print("String1 - ");
    Serial.print(stringVar1.getValue());
    Serial.print(" Address - ");
    Serial.print(stringVar1.getAddres());
    Serial.println();

    Serial.print("String2 - ");
    Serial.print(stringVar2.getValue());
    Serial.print(" Address - ");
    Serial.print(stringVar2.getAddres());
    Serial.println();

    Serial.print("String3 - ");
    Serial.print(stringVar3.getValue());
    Serial.print(" Address - ");
    Serial.print(stringVar3.getAddres());
    Serial.println();

    Serial.print("String4 - ");
    Serial.print(stringVar4.getValue());
    Serial.print(" Address - ");
    Serial.print(stringVar4.getAddres());
    Serial.println();

    Serial.print("String5 - ");
    Serial.print(stringVar5.getValue());
    Serial.print(" Address - ");
    Serial.print(stringVar5.getAddres());
    Serial.println();
    Serial.println();
    Serial.println();
    startTime = millis();
  }

  if (_bounseInputD2O)
  {

    if (!oldValue)
    {
      oldValue = true;

      byteVar2.setValue(100);
      intVar2.setValue(101);
      longVar2.setValue(102);
      unLongVar2.setValue(103);
      floatVar2.setValue(104.5);
      boolVar1.setValue(false);
      boolVar2.setValue(true);
      boolVar3.setValue(false);
      boolVar4.setValue(true);
      boolVar5.setValue(false);
      boolVar6.setValue(true);
      boolVar7.setValue(false);
      boolVar8.setValue(true);
      boolVar9.setValue(false);
      boolVar10.setValue(true);

    }
    else
    {
      oldValue = false;
    }
  }
}