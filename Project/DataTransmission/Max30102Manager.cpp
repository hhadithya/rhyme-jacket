#include "Max30102Manager.h"

Max30102Manager::Max30102Manager() 
    : rateSpot(0), lastBeat(0), beatsPerMinute(0), beatAvg(0), lastPrintTime(0), lpfConstant(0.9), filteredBPM(0) {}

void Max30102Manager::initializeSensor() {
    Serial.begin(115200);
    Serial.println("Initializing...");

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("MAX30105 was not found. Please check wiring/power.");
        while (1);
    }
  
    particleSensor.setup(); 
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
}

int Max30102Manager::getBPM() {
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true) {
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute; 
            rateSpot %= RATE_SIZE; 

            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++)
                beatAvg += rates[x];
            beatAvg /= RATE_SIZE;

            filteredBPM = (lpfConstant * filteredBPM) + ((1 - lpfConstant) * beatAvg);
        }
    }

    if (millis() - lastPrintTime >= 1000) {
        lastPrintTime = millis();

        // Serial.print("IR=");
        // Serial.print(irValue);
        // Serial.print(", BPM=");
        // Serial.print(beatsPerMinute);
        // Serial.print(", Avg BPM=");
        // Serial.print(beatAvg);
        // Serial.print(", Filtered BPM=");
        // Serial.print(filteredBPM);

        // if (irValue < 50000)
        //     Serial.print(" No finger?");

        // Serial.println();
    }

    return filteredBPM;
}
