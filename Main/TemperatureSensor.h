#ifndef ONEWIRESENSOR_H
#define ONEWIRESENSOR_H

#include <OneWire.h>

#define ONE_WIRE_BUS 32
OneWire oneWire(ONE_WIRE_BUS);

float getTemperature() {
  byte data[12];
  byte addr[8];

  if (!oneWire.search(addr)) {
    oneWire.reset_search();
    return NAN;
  }

  if (OneWire::crc8(addr, 7) != addr[7] || addr[0] != 0x28) {
    return NAN;
  }

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1); // Start temperature conversion
  delay(1000); // Wait for conversion to complete

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) {
    data[i] = oneWire.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  float celsius = (float)raw / 16.0;

  return celsius;
}

#endif
