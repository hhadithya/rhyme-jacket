#include "BluetoothMouse.h"
// #include <Arduino.h>  // Include this header for Serial communication

BluetoothMouse::BluetoothMouse() {
}

void BluetoothMouse::begin() {
    Wire.begin();

    uint8_t i2cData[4] = {7, 0x00, 0x00, 0x00};

    while(i2cWrite(0x19, i2cData, 4, false));
    while(i2cWrite2(0x6B, 0x01, true));
    while(i2cRead(0x75, i2cData, 1));
    delay(100);
    while(i2cRead(0x3B, i2cData, 6));

    timer = micros();
    // Serial.begin(115200);
    bleMouse.begin();
    delay(100);
}

void BluetoothMouse::update() {
    while(i2cRead(0x3B, data, 14));

    gyroX = ((data[8] << 8) | data[9]);
    gyroY = ((data[10] << 8) | data[11]);

    gyroX = gyroX / Sensitivity / 1.1 * -1;
    gyroY = gyroY / Sensitivity * -1;

    if(bleMouse.isConnected()) {
        // Serial.print(gyroX);
        // Serial.print("   ");
        // Serial.print(gyroY);
        // Serial.print("\r\n");
        bleMouse.move(gyroY, -gyroX);
    }
    delay(delayi);
}

uint8_t BluetoothMouse::i2cWrite(uint8_t registerAddress, uint8_t* data, uint8_t length, bool sendStop) {
    Wire.beginTransmission(IMUAddress);
    Wire.write(registerAddress);
    Wire.write(data, length);
    return Wire.endTransmission(sendStop); // Returns 0 on success
}

uint8_t BluetoothMouse::i2cWrite2(uint8_t registerAddress, uint8_t data, bool sendStop) {
    return i2cWrite(registerAddress, &data, 1, sendStop); // Returns 0 on success
}

uint8_t BluetoothMouse::i2cRead(uint8_t registerAddress, uint8_t* data, uint8_t nbytes) {
    uint32_t timeOutTimer;
    Wire.beginTransmission(IMUAddress);
    Wire.write(registerAddress);
    if(Wire.endTransmission(false))
        return 1;
    Wire.requestFrom(IMUAddress, nbytes, (uint8_t)true);
    for(uint8_t i = 0; i < nbytes; i++) {
        if(Wire.available())
            data[i] = Wire.read();
        else {
            timeOutTimer = micros();
            while(((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available());
            if(Wire.available())
                data[i] = Wire.read();
            else
                return 2;
        }
    }
    return 0;
}
