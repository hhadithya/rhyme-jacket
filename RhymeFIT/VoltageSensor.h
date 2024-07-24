#define ANALOG_IN_PIN  33 // ESP32 pin GPIO36 (ADC0) connected to voltage sensor
#define REF_VOLTAGE    3.3
#define ADC_RESOLUTION 4096.0
#define R1             29700.0 // resistor values in voltage sensor (in ohms)
#define R2             7400.0  // resistor values in voltage sensor (in ohms)

float avg_voltage = 0;
const float MIN_VOLTAGE = 5;  // Minimum voltage corresponding to 0% battery level
const float MAX_VOLTAGE = 9.6;  // Maximum voltage corresponding to 100% battery level

void setupVoltageSensor() {

  for (int i = 0; i < 1000; i++) {
    int adc_value = analogRead(ANALOG_IN_PIN);

    // determine voltage at adc input
    float voltage_adc = ((float)adc_value * REF_VOLTAGE) / ADC_RESOLUTION;

    // calculate voltage at the sensor input
    float voltage_in = voltage_adc * (R1 + R2) / R2;

    avg_voltage += voltage_in;
    delay(1);
  }

  avg_voltage = (avg_voltage/1000) + 0.7;
}

float loopVoltageSensor() {
  // calculate battery level percentage
  float battery_percentage = ((avg_voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0;

  // constrain the battery percentage between 0 and 100
  battery_percentage = constrain(battery_percentage, 0, 100);

  // print results to seri  al monitor
  // Serial.print("Measured Voltage = ");
  // Serial.print(avg_voltage, 2);
  // Serial.print(" V, Battery Level = ");
  // Serial.print(battery_percentage, 2);
  // Serial.println(" %");
  
  return battery_percentage;
}
