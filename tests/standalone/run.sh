#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../../"
stubs="tests/standalone/stubs"
files="tests/standalone/mocks.cpp src/flprogEEPROM.cpp src/flprogExternalEEPROM.cpp"
for plat in avr esp rp2040 stm32; do
  case "$plat" in
    avr) define=ARDUINO_ARCH_AVR;variant=avr/flprogAvrInternalEEPROM.cpp;;
    esp) define=ARDUINO_ARCH_ESP32;variant=esp/flprogEspInternalEEPROM.cpp;;
    rp2040) define=ARDUINO_ARCH_RP2040;variant=rp2040/flprogRP2040InternalEEPROM.cpp;;
    stm32) define=ARDUINO_ARCH_STM32;variant=stm32/flprogStm32InternalEEPROM.cpp;;
  esac
  g++ -std=c++11 -Wall -Wextra -Werror -g -D"$define" -I"$stubs" -Isrc $files "src/variant/$variant" tests/standalone/test.cpp -o "/tmp/flprog-eeprom-test-$plat"
  "/tmp/flprog-eeprom-test-$plat"
done
