#ifndef MPUMANAGER_H
#define MPUMANAGER_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>

class MPUManager {
public:
    MPUManager();
    int initializeMPU(bool center_check);
    int* getMPUAngles();
    void getRawGyroValues(int &gx, int &gy, int &gz);

private:
    MPU6050 mpu;
    bool dmpReady;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];
    Quaternion q;
    VectorFloat gravity;
    VectorInt16 gyro;
    float ypr[3];
    bool blinkState;

    static void dmpDataReady();
    static volatile bool mpuInterrupt;
};

#endif
