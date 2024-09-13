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
    delay(10);
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
  int flexValue = analogRead(pin), flexAngle;

  // Serial.printf("|flex %d : %d\n", pin, flexValue);

  // Map the sensor value to 0-150 degree range
  if (pin == 34){
    flexAngle = map(flexValue, initial, 1750, 0, 150);
  }else if(pin == 35){
    flexAngle = map(flexValue, initial, 771, 0, 150);
  }

  // Ensure the value is within 0-150 degree range
  flexAngle = constrain(flexAngle, 0, 150);

  return flexAngle;
}

#endif