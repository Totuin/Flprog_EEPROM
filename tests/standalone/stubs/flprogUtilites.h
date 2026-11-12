#pragma once
#include "Arduino.h"
namespace flprog {
inline uint32_t timeBack(uint32_t ms) { return millis() - ms; }
inline bool isTimer(uint32_t begin, uint32_t period) { return uint32_t(millis() - begin) >= period; }
}
