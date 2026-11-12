#pragma once
#include "Arduino.h"
struct MockEEPROM {
 std::vector<uint8_t> mem;
 int writes=0, commits=0, begins=0;
 MockEEPROM() : mem(256,255) {}
 bool begin(size_t len) {begins++; if (len > mem.size()) mem.resize(len,255); return true;}
 uint16_t length() const {return (uint16_t)mem.size();}
 uint8_t read(size_t n) const {return mem.at(n);}
 void write(size_t n,uint8_t v) {mem.at(n) = v; writes++;}
 bool commit() {commits++;return true;}
 void reset(size_t n=256,uint8_t fill=255) { mem.assign(n, fill); writes=commits=begins=0;}
};
extern MockEEPROM EEPROM;
inline uint8_t eeprom_read_byte(uint16_t n) {return EEPROM.read(n);}
inline uint8_t eeprom_buffered_read_byte(uint16_t n) {return EEPROM.read(n);}
inline void eeprom_buffered_write_byte(uint16_t n,uint8_t value) {EEPROM.write(n,value);}
inline void eeprom_buffer_flush() {EEPROM.commit();}
