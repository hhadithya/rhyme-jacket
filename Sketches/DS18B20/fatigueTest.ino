#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define LED_BUILTIN 13

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

  if(Celsius >= 34.50){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }

  if(Celsius < 34.50){
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(500);
}
