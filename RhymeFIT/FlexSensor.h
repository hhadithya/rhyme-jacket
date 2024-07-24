#ifndef FLEXSENSOR_H
#define FLEXSENSOR_H

#define FLEX_SENSOR_1_PIN 34
#define FLEX_SENSOR_2_PIN 35
#define NUM_CALIBRATION_READINGS 500  // Number of readings for averaging

int flex1Initial, flex2Initial;

void calibrateFlex() {
  long flex1Sum = 0;
  long flex2Sum = 0;
  
  // Take multiple readings for averaging
  for (int i = 0; i < NUM_CALIBRATION_READINGS; i++) {
    flex1Sum += analogRead(FLEX_SENSOR_1_PIN);
    flex2Sum += analogRead(FLEX_SENSOR_2_PIN);
    delay(10);  // Small delay between readings for stability
  }

  // Calculate the average initial values
  flex1Initial = flex1Sum / NUM_CALIBRATION_READINGS;
  flex2Initial = flex2Sum / NUM_CALIBRATION_READINGS;

  Serial.print("Flex Sensor 1 Initial Value (Average): ");
  Serial.println(flex1Initial);
  Serial.print("Flex Sensor 2 Initial Value (Average): ");
  Serial.println(flex2Initial);
}

int getFlexAngle(int pin, int initial) {
  // Read the sensor value
  int flexValue = analogRead(pin);

  // Map the sensor value to 0-150 degree range
  int flexAngle = map(flexValue, initial, 4095, 0, 180);

  // Ensure the value is within 0-150 degree range
  flexAngle = constrain(flexAngle, 0, 180);

  return flexAngle;
}

#endif