#ifndef MAX30102MANAGER_H
#define MAX30102MANAGER_H

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

class Max30102Manager {
public:
    Max30102Manager();
    void initializeSensor();
    int getBPM();

private:
    MAX30105 particleSensor;
    const byte RATE_SIZE = 10;
    byte rates[10];
    byte rateSpot;
    long lastBeat;
    float beatsPerMinute;
    int beatAvg;
    unsigned long lastPrintTime;
    float lpfConstant;
    int filteredBPM;
};

#endif // MAX30102MANAGER_H
