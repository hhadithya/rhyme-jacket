#include "MPUManager.h"

volatile bool MPUManager::mpuInterrupt = false;

MPUManager::MPUManager() : mpu(), dmpReady(false), blinkState(false) {}

int MPUManager::initializeMPU(bool center_check) {
    Wire.begin();
    Wire.setClock(400000);

    mpu.initialize();
    delay(100);
    Serial.print(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    delay(250);

    devStatus = mpu.dmpInitialize();

    if (center_check){
      mpu.setXGyroOffset(220);
      mpu.setZGyroOffset(100);
      mpu.setYGyroOffset(-85);
      mpu.setYAccelOffset(1788); 
    }else{
      mpu.setXGyroOffset(220);
      mpu.setYGyroOffset(76);
      mpu.setZGyroOffset(-85);
      mpu.setZAccelOffset(1788); 
    }



    if (devStatus == 0) {
        mpu.CalibrateAccel(10);
        mpu.CalibrateGyro(10);
        mpu.PrintActiveOffsets();

        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        mpuIntStatus = mpu.getIntStatus();

        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
        delay(500);
        return 1;
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
        return 0;
    }
}


int* MPUManager::getMPUAngles() {
    static int angles[3]; // static array to hold angles

    if (!dmpReady) return nullptr;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { 
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        
        angles[0] = (int)(ypr[0] * 180 / M_PI);
        angles[1] = (int)(ypr[1] * 180 / M_PI);
        angles[2] = (int)(ypr[2] * 180 / M_PI);

        blinkState = !blinkState;

        return angles; 
    }

    return nullptr;
}

void MPUManager::dmpDataReady() {
    mpuInterrupt = true;
} 

void MPUManager::getRawGyroValues(int &gx, int &gy, int &gz) {
    if (!dmpReady) return;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        int16_t rawData[3];
        mpu.dmpGetGyro(rawData, fifoBuffer);

        // Serial.print("FIFO Raw Gyro X: "); Serial.println(rawData[0]);
        // Serial.print("FIFO Raw Gyro Y: "); Serial.println(rawData[1]);
        // Serial.print("FIFO Raw Gyro Z: "); Serial.println(rawData[2]);

        gx = rawData[0];
        gy = rawData[1];
        gz = rawData[2];
    }
}

