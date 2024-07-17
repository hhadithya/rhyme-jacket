#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 32;    
const int flexPin1 = 34; 
const int flexPin2 = 35;  

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


void flexData(){
  int flexValue1, flexValue2;
  
  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  
  Serial.print("sensor1: ");
  Serial.print(flexValue1);
  Serial.print("| sensor2: ");
  Serial.println(flexValue2);
}

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.print("ºC | ");
  Serial.print(temperatureF);
  Serial.println("ºF");
  flexData();
  delay(500);
}