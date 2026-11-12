#include "Arduino.h"
#include "EEPROM.h"
#include "RT_HW_BASE.h"
#include "flprogInternalEEPROM.h"
#include "flprogExternalEEPROM.h"
#include <cassert>
#include <cstring>
#include <iostream>

void common(FLProgAbstractEEPROM& obj) {
 obj.saveByte(7,0xAB); assert(obj.readByte(7)==0xAB);
 obj.saveBoolean(7,0,true); assert(obj.readBoolean(7,0));
 uint8_t tail[] = {7,8};
 obj.saveByteArray(6,2,tail);uint8_t got[]={0,0};obj.readByteArray(6,2,got);
 assert(got[0]==7 && got[1]==8);
 obj.saveByteArray(7,2,tail); assert(obj.readByte(7)==8);
 obj.saveByte(0,0xFF); assert(obj.readByte(0)==0);
 obj.saveInteger(6,0x1234); assert(obj.readInteger(6)==0x1234);
 obj.saveByte(7,0);assert(obj.readInteger(7)==0); // off-by-end
 obj.saveLong(1,0x12345678);assert(obj.readLong(1)==0x12345678);
 obj.saveUnsignedLong(1,0xFEDCBA98u);assert(obj.readUnsignedLong(1)==0xFEDCBA98u);
 obj.saveFloat(1,12.5f);assert(obj.readFloat(1)==12.5f);
 obj.saveString(1,6,"hi");assert(obj.readString(1,6)=="hi");
 uint8_t prefix[]={3,4}; obj.saveByteArray(1,0,prefix);
 obj.saveByteArray(1,2,nullptr);obj.readByteArray(1,2,nullptr);
 uint8_t sentinel=75;obj.readByteArray(7,2,&sentinel);assert(sentinel==75);
 obj.saveUint32Array(2,65535,(uint32_t*)&sentinel); // invalid range must not dereference
 uint32_t arr32=0;obj.readUint32Array(2,65535,&arr32);
 obj.saveUint16Array(6,1,(uint16_t*)&arr32); uint16_t x=0;obj.readUint16Array(6,1,&x);
 obj.saveByteArray(65535,2,prefix);
}
void testInternal() {
 EEPROM.reset(256,255);
 { FLProgInternalEEPROM zero(0,11);zero.begin();zero.pool();assert(zero.readByte(1)==0); }
 {
   FLProgInternalEEPROM obj(8,42);
   obj.setByteStartValue(1,99);
   obj.begin();
   assert(obj.readByte(1)==99);assert(obj.readByte(2)==0);
   common(obj);
   for(int i=0;i<20;i++) obj.pool();
   assert(EEPROM.read(0)==42 && EEPROM.read(1)==uint8_t('h'));
 }
 {
   FLProgInternalEEPROM obj(8,42);obj.begin();
   assert(obj.readByte(2)==uint8_t('i')); // restored RAM from EEPROM
 }
}
void testExternal() {
 RT_HW_Base.reset(64);mockMillis=100;
 // Неизвестный маркер при неудачном чтении не должен приводить к стиранию памяти.
 {
  RT_HW_Base.error=true;
  FLProgExternalEEPROM interrupted(0,0x50,8,42);
  interrupted.begin();
  assert(RT_HW_Base.writes==0 && RT_HW_Base.mem[0]==255);
  RT_HW_Base.error=false;
 }

 {FLProgExternalEEPROM empty(0,0x50,0,17);empty.begin();empty.pool();assert(empty.readByte(1)==0);}
 {
   FLProgExternalEEPROM obj(0,0x50,8,42);
   obj.setByteStartValue(1,99);
   obj.begin();assert(obj.readByte(1)==99);
   common(obj);
   for(int i=0;i<20;i++) {mockMillis+=10;obj.pool();}
   assert(RT_HW_Base.mem[0]==42);
   obj.saveByte(1,84);
   RT_HW_Base.error=true;mockMillis+=10;obj.pool();
   assert(RT_HW_Base.mem[1]!=84);
   RT_HW_Base.error=false;mockMillis+=10;obj.pool();
   assert(RT_HW_Base.mem[1]==84); // failed I2C transaction is retried
 }
 // В 24C0X нельзя адресовать ячейки свыше 255 через один адресный байт.
 RT_HW_Base.reset(512);RT_HW_Base.shortAddress=true;
 {
   FLProgExternalEEPROM obj(0,0x50,500,42,FLPROG_24C0X_EXTERNAL_EEPROM);
   obj.begin();
   obj.saveByte(255,0x55);obj.saveByte(256,0x66);
   for(int i=0;i<260;i++) {mockMillis+=10;obj.pool();}
   assert(obj.readByte(255)==0x55 && obj.readByte(256)==0);
   assert(RT_HW_Base.mem[255]==0x55);
 }
}
int main() {testInternal();testExternal();std::cout<<"EEPROM_STANDALONE_PASS"<<std::endl;}
