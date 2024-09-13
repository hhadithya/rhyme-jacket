#ifndef BLUETOOTHMOUSE_H
#define BLUETOOTHMOUSE_H

#include <Wire.h>
#include <BleMouse.h>

class BluetoothMouse {
public:
    BluetoothMouse();
    void begin();
    void update();

private:
    uint8_t data[14];
    int16_t gyroX, gyroY;
    const int Sensitivity = 600;
    const int delayi = 20;
    BleMouse bleMouse;
    const uint8_t IMUAddress = 0x68;
    const uint16_t I2C_TIMEOUT = 1000;
    uint8_t i2cWrite(uint8_t registerAddress, uint8_t* data, uint8_t length, bool sendStop);
    uint8_t i2cWrite2(uint8_t registerAddress, uint8_t data, bool sendStop);
    uint8_t i2cRead(uint8_t registerAddress, uint8_t* data, uint8_t nbytes);
    uint32_t timer;
};

#endif
