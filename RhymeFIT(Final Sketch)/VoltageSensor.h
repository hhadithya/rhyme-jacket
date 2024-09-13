#define ANALOG_IN_PIN  33
#define REF_VOLTAGE    3.3
#define ADC_RESOLUTION 4096.0
#define R1             29700.0
#define R2             7400.0 

float avg_voltage = 0;
const float MIN_VOLTAGE = 5;
const float MAX_VOLTAGE = 8;

void setupVoltageSensor() {

  for (int i = 0; i < 1000; i++) {
    int adc_value = analogRead(ANALOG_IN_PIN);

    float voltage_adc = ((float)adc_value * REF_VOLTAGE) / ADC_RESOLUTION;

    float voltage_in = voltage_adc * (R1 + R2) / R2;

    avg_voltage += voltage_in;
    delay(1);
  }

  avg_voltage = (avg_voltage/1000) + 0.7;
}

float loopVoltageSensor() {

  float battery_percentage = ((avg_voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0;

  // constrain the battery percentage between 0 and 100
  battery_percentage = constrain(battery_percentage, 0, 100);

  // Serial.print("Measured Voltage = ");
  // Serial.print(avg_voltage, 2);
  // Serial.print(" V, Battery Level = ");
  // Serial.print(battery_percentage, 2);
  // Serial.println(" %");
  
  return battery_percentage;
}
