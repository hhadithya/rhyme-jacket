#ifndef MPU6050SENSOR_H
#define MPU6050SENSOR_H

#include <Wire.h>
#include <math.h>

float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
float AccX, AccY, AccZ;
float RateCalibrationAccX, RateCalibrationAccY, RateCalibrationAccZ;
float AngleRoll, AnglePitch;

void gyro_signals() {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();
  RateRoll = (float) GyroX / 65.5;
  RatePitch = (float) GyroY / 65.5;
  RateYaw = (float) GyroZ / 65.5;

  AccX = (float)AccXLSB / 4096;
  AccY = (float)AccYLSB / 4096;
  AccZ = (float)AccZLSB / 4096;
}

void setupMPU6050() {
  Wire.begin();
  Wire.setClock(400000);
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  for (RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++) {
    gyro_signals();
    RateCalibrationAccX += AccX;
    RateCalibrationAccY += AccY;
    RateCalibrationAccZ += AccZ;
    delay(1);
  }

  RateCalibrationAccX /= 2000;
  RateCalibrationAccY /= 2000;
  RateCalibrationAccZ /= 2000;
}

void readMPU6050() {
  gyro_signals();

  AccX -= RateCalibrationAccX;
  AccY -= RateCalibrationAccY;

  AngleRoll = atan(AccY / sqrt(AccX * AccX + AccZ * AccZ)) * 180.0 / PI;
  AnglePitch = -atan(AccX / sqrt(AccY * AccY + AccZ * AccZ)) * 180.0 / PI;

  Serial.print("AngleRoll:");
  Serial.println(AngleRoll);
  
  Serial.print("AnglePitch:");
  Serial.println(AnglePitch);
}

#endif
