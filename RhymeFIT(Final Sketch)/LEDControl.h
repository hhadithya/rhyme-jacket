#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN1     5
#define LED_PIN2     15
#define NUM_LEDS     14
#define BRIGHTNESS   64

extern Adafruit_NeoPixel strip1;
extern Adafruit_NeoPixel strip2;

void situationBodyChecking(uint16_t duration);
void setupLEDs();
void loopLEDs();
void situationGoodToGo();
void situationCalibration();
void situationWin();
void situationLose();
void situationLevel1();
void situationLevel2();
uint32_t blend(uint32_t color1, uint32_t color2, uint16_t step, uint16_t totalSteps);

#endif