#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <Wire.h>

class Multiplexer {
public:
    Multiplexer(uint8_t address);
    void selectChannel(uint8_t channel);

private:
    uint8_t _address;
};

#endif // MULTIPLEXER_H
