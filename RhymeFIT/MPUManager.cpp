#include "MPUManager.h"

// #define INTERRUPT_PIN 2
// #define LED_PIN 2

volatile bool MPUManager::mpuInterrupt = false;

MPUManager::MPUManager() : mpu(), dmpReady(false), blinkState(false) {}

void MPUManager::initializeMPU() {
    Wire.begin();
    Wire.setClock(400000);

    mpu.initialize();
    // pinMode(INTERRUPT_PIN, INPUT);

    Serial.print(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); 

    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();

        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        // Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        // Serial.println(F(")..."));
        // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
        delay(500);
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
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
        // digitalWrite(LED_PIN, blinkState);

        return angles; 
    }

    return nullptr;
}

void MPUManager::dmpDataReady() {
    mpuInterrupt = true;
}
