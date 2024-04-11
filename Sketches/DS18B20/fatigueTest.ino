#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
// #define LED_BUILTIN 13

float bodyTemp;
float environmentTemp;

void ledOn(int pin){
  digitalWrite(pin, HIGH);  // turn the LED on 
  delay(500);
  digitalWrite(pin, LOW);   // turn the LED off
  delay(200);
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
}

void ledOff(){
  digitalWrite(8, HIGH);  // turn the LED on
  delay(500);
  digitalWrite(8, LOW);   // turn the LED off
  delay(200);
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
}

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;

Adafruit_MPU6050 mpu;

void setup(void) {
  sensors.begin();
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  pinMode(LED_BUILTIN, OUTPUT); // LED 

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sensors.requestTemperatures();

  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  /* Print out the values */
  Serial.print("Environment Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.print("Body temperature: ");
  Serial.print(Celsius);
  Serial.println(" C  ");

  Serial.println("");

  bodyTemp = Celsius;
  environmentTemp = temp.temperature;

  // Simple fatigue estimation logic with consideration for cold environments
  int fatigueLevel = 0;
  if (bodyTemp < 36.0 && environmentTemp < 31) {
    fatigueLevel = 2; // Low body temp + cold environment = more tired
  } else if (bodyTemp < 36.0) {
    fatigueLevel = 1; // Low body temp = somewhat tired
  } else if (bodyTemp > 37.0 && environmentTemp > 33) {
    fatigueLevel = 2; // High body temp + hot environment = more tired
  } else if (bodyTemp > 37.0) {
    fatigueLevel = 1; // High body temp = somewhat tired
  } else {
    fatigueLevel = 0; // Normal body temp = less likely tired
  }

  //switching on LEDs according to the fatigue level
  switch(fatigueLevel){
    case 2:
      ledOn(12); //when fatigue level is 2 red LED is blinking 
      break;
    case 1:
      ledOn(13); //when fatigue level is 1 blue LED is blinking
      break;
    case 0:
      ledOff(); //when fatigue level is 0 green LED is blinking
  }

  // if(Celsius >= 34.50){
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   delay(500);                      // wait for a second
  //   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   delay(200);
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // }

  // if(Celsius < 34.50){
  //   digitalWrite(LED_BUILTIN, LOW);
  // }

  delay(500);
}
