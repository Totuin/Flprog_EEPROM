#pragma once
#include "Arduino.h"
#include "EEPROM.h"
struct RT_HW_STRUCT_I2C_DEV { int link=0; int codeErr=0; uint8_t bf8=0; int status=0; uint8_t addr=0; uint8_t bus=0; uint16_t cursor=0; };
struct MockRTBase {
 std::vector<uint8_t> mem;
 bool error=false, present=true;
 bool shortAddress=false;
 int writes=0;
 MockRTBase(): mem(512,255) {}
 void reset(size_t n=512,uint8_t fill=255) {mem.assign(n,fill);error=false;present=true;shortAddress=false;writes=0;}
 void i2cSetParam(RT_HW_STRUCT_I2C_DEV &d,uint8_t addr,uint8_t bus,uint8_t) {d.addr=addr;d.bus=bus;}
 void i2cInitDevice(RT_HW_STRUCT_I2C_DEV &d) {d.link=present?1:0;d.codeErr=present?0:1;}
 void i2cWrite(RT_HW_STRUCT_I2C_DEV &d,uint8_t addr) {d.codeErr=error?1:0;d.cursor=addr;}
 void i2cRead(RT_HW_STRUCT_I2C_DEV &d) {d.codeErr=error?1:0;if (!error) d.bf8=mem.at(d.cursor);}
 void i2cWriteArr(RT_HW_STRUCT_I2C_DEV &d,uint8_t* arr,uint16_t n) {
   d.codeErr=error?1:0; if(error) return;
   if(n==3) {d.cursor=(uint16_t(arr[0])<<8)|arr[1];mem.at(d.cursor)=arr[2];writes++;}
   if(n==2) {
     if(shortAddress) {d.cursor=arr[0];mem.at(d.cursor)=arr[1];writes++;}
     else d.cursor=(uint16_t(arr[0])<<8)|arr[1];
   }
 }
};
extern MockRTBase RT_HW_Base;
