#include <OneWire.h>

const int oneWireBus =25;
OneWire oneWire(oneWireBus);

float readTemperature() {
  byte data[9];
  byte addr[8];

  if (!oneWire.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    oneWire.reset_search();
    delay(250);
    return -1000; 
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000; 
  }

  if (addr[0] != 0x28) {
    Serial.println("Device is not a DS18B20 family device.");
    return -1000; 
  }

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1); 

  delay(750); 

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); 

  for (int i = 0; i < 9; i++) {
    data[i] = oneWire.read();
  }

  oneWire.reset_search();


  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);


  if (cfg == 0x00) raw = raw & ~7;  
  else if (cfg == 0x20) raw = raw & ~3;
  else if (cfg == 0x40) raw = raw & ~1; 

  float celsius = (float)raw / 16.0;
  return celsius;
}

float getAverageTemperature(int numReadings, int delayBetweenReadings) {
  float totalTemperatureC = 0;

  for (int i = 0; i < numReadings; i++) {
    float tempC = readTemperature();
    if (tempC != -1000) { 
      totalTemperatureC += tempC;
    }
    delay(delayBetweenReadings); 
  }

  return totalTemperatureC / numReadings; 
}

void tempLoop(){
  int numReadings = 2; 
  int delayBetweenReadings = 500; 

  float averageTemperatureC = getAverageTemperature(numReadings, delayBetweenReadings);
  float averageTemperatureF = averageTemperatureC * 9.0 / 5.0 + 32.0;

  Serial.print("Average Temperature: ");
  Serial.print(averageTemperatureC);
  Serial.print("ºC  ");
  Serial.print(averageTemperatureF);
  Serial.println("ºF"); 
}