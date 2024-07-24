#ifndef MPUMANAGER_H
#define MPUMANAGER_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>

class MPUManager {
public:
    MPUManager();
    void initializeMPU();
    int* getMPUAngles();

private:
    MPU6050 mpu;
    bool dmpReady;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint8_t fifoBuffer[64];
    Quaternion q;
    VectorFloat gravity;
    float ypr[3];
    bool blinkState;

    static void dmpDataReady();
    static volatile bool mpuInterrupt;
};

#endif // MPUMANAGER_H
