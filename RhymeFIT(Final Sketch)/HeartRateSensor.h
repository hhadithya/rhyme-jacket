#ifndef HEART_RATE_SENSOR_H
#define HEART_RATE_SENSOR_H

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "Multiplexer.h"

class HeartRateSensor {
public:
    HeartRateSensor(uint8_t tcaChannel, Multiplexer& mux);
    bool begin();
    void update();
    float getFilteredBPM() const;

private:
    MAX30105 particleSensor;
    Multiplexer& multiplexer;
    uint8_t tcaChannel;

    const byte RATE_SIZE = 5;
    byte rates[5];
    byte rateSpot = 0;
    long lastBeat = 0; // Time at which the last beat occurred

    float beatsPerMinute;
    int beatAvg;

    // Low-pass filter variables
    float lpfConstant = 0.9;
    float filteredBPM = 0;
};

#endif
