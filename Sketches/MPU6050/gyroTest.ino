float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
float AccX, AccY, AccZ;
float RateCalibrationAccX, RateCalibrationAccY, RateCalibrationAccZ;
float AngleRoll, AnglePitch;


void gyro_signals(void){
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
  int16_t GyroX = Wire.read()<<8 | Wire.read();
  int16_t GyroY = Wire.read()<<8 | Wire.read();
  int16_t GyroZ = Wire.read()<<8 | Wire.read();
  RateRoll = (float) GyroX / 65.5;
  RatePitch = (float) GyroY / 65.5;
  RateYaw = (float) GyroZ / 65.5;

  AccX = (float)AccXLSB / 4096;
  AccY = (float)AccYLSB / 4096;
  AccZ = (float)AccZLSB / 4096;

}


void setup(){
  Serial.begin(57600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  // for(RateCalibrationNumber = 0; RateCalibrationNumber<2000; RateCalibrationNumber++){
  //   gyro_signals();
  //   RateCalibrationRoll += RateRoll;
  //   RateCalibrationPitch += RatePitch;
  //   RateCalibrationYaw += RateYaw;
  //   delay(1);
  // }

  // RateCalibrationRoll /= 2000;
  // RateCalibrationPitch /= 2000;
  // RateCalibrationYaw /= 2000;

  for (RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++){
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


void loop(){
  gyro_signals();
  // RateRoll -= RateCalibrationRoll;
  // RatePitch -= RateCalibrationPitch;
  // RateYaw -= RateCalibrationYaw;

  // Serial.print("Roll rate[°/s] = ");
  // Serial.print(RateRoll);
  // Serial.print(" | Pitch rate[°/s] = ");
  // Serial.print(RatePitch);
  // Serial.print(" | Yaw rate[°/s] = ");
  // Serial.print(RateYaw);
  // Serial.println("");
  
  AccX -= RateCalibrationAccX;
  AccY -= RateCalibrationAccY;

  AngleRoll = atan(AccY/sqrt(AccX*AccX + AccZ*AccZ))*1/(3.142 / 180);
  AnglePitch = -atan(AccX/sqrt(AccY*AccY + AccZ*AccZ))*1/(3.142/180);

  // Serial.print("Acceleration X = ");
  // Serial.print(AccX);
  // Serial.print(" | Acceleration Y = ");
  // Serial.print(AccY);
  // Serial.print(" | Acceleration Z = ");
  // Serial.print(AccZ);
  Serial.print("Roll angle[°] = ");
  Serial.print(AngleRoll);
  Serial.print(" | Pitch angle[°] = ");
  Serial.print(AnglePitch);
  Serial.println("");

}