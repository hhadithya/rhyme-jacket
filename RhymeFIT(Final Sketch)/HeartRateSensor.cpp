#include "HeartRateSensor.h"

#define TCA_ADDRESS 0x70 // Default address for TCA9548A

HeartRateSensor::HeartRateSensor(uint8_t tcaChannel, Multiplexer& mux)
    : tcaChannel(tcaChannel), multiplexer(mux), beatsPerMinute(0), beatAvg(0), filteredBPM(0) {
    for (byte i = 0; i < RATE_SIZE; i++) {
        rates[i] = 0;
    }
}

bool HeartRateSensor::begin() {
    multiplexer.selectChannel(tcaChannel);

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("MAX30105 was not found. Please check wiring/power.");
        return false;
    }

    Serial.println("Place your index finger on the sensor with steady pressure.");

    particleSensor.setup(); // Configure sensor with default settings
    particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); // Turn off Green LED
    
    return true;
}

void HeartRateSensor::update() {
    multiplexer.selectChannel(tcaChannel);
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true) {
        
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
            rateSpot %= RATE_SIZE; 

            // Take average of readings
            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++)
                beatAvg += rates[x];
            beatAvg /= RATE_SIZE;

            // Apply low-pass filter
            filteredBPM = (lpfConstant * filteredBPM) + ((1 - lpfConstant) * beatAvg);
        }
    }

    // Print output once per second
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000) {
        lastPrintTime = millis();
        // Serial.print("Filtered BPM=");
        // Serial.print(filteredBPM);

        if (irValue < 50000)
            Serial.print(" No finger?");
    }
}

float HeartRateSensor::getFilteredBPM() const {
    return filteredBPM;
}
