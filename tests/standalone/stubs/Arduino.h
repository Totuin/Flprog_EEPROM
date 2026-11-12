#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <cstring>
class String {
public:
  std::string text;
  String() = default;
  String(const char* value): text(value ? value : "") {}
  size_t length() const { return text.size(); }
  char charAt(size_t i) const { return i < text.size() ? text[i] : 0; }
  bool concat(char ch) { text += ch; return true; }
  bool operator==(const char* other) const {return text == other;}
};
#define bitRead(value, bit) (((value) >> (bit)) & 1u)
#define bitWrite(value, bit, bitValue) do { if (bitValue) (value) |= uint8_t(1u << (bit)); else (value) &= uint8_t(~(1u << (bit))); } while(0)
#define highByte(value) (uint8_t(((value) >> 8) & 255))
#define lowByte(value) (uint8_t((value) & 255))
inline uint16_t word(uint8_t hi, uint8_t lo) {return uint16_t((uint16_t(hi) << 8) | lo);}
extern uint32_t mockMillis;
inline uint32_t millis() { return mockMillis; }
inline void delay(uint32_t ms) { mockMillis += ms; }
