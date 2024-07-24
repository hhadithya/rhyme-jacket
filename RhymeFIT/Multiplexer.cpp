#include "Multiplexer.h"

Multiplexer::Multiplexer(uint8_t address) : _address(address) {
    Wire.begin();
}

void Multiplexer::selectChannel(uint8_t channel) {
    if (channel > 7) return;
    Wire.beginTransmission(_address);
    Wire.write(1 << channel);
    Wire.endTransmission();
}
