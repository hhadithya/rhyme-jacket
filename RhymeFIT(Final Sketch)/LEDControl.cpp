#include "LEDControl.h"

// Initialize the LED strips
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setupLEDs() {
  // Set up the LED strips
  strip1.begin();
  strip2.begin();
  strip1.setBrightness(BRIGHTNESS);
  strip2.setBrightness(BRIGHTNESS);
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
}

void loopLEDs() {
  // Call the desired function here
  situationBodyChecking(500);
  delay(2000);
  situationGoodToGo();
  delay(2000);
  // situationBodyChecking();
  // delay(2000);
  situationCalibration();
  delay(2000);
  situationWin();
  delay(2000);
  situationLose();
  delay(2000);
  situationLevel1();
  delay(2000);
  situationLevel2();
  delay(2000);
}


void setStripBrightness(Adafruit_NeoPixel& strip, int brightness) {
  // Set the color to a bluish hue, adjusting brightness for the blue component
  int blueValue = brightness; // Full blue
  int redValue = brightness / 4; // Less red for a bluish hue
  int greenValue = brightness / 8; // Even less green for a bluish hue

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue));
  }
  strip.show();
}

void situationBodyChecking(uint16_t duration) {
  for (int brightness = 0; brightness <= 255; brightness++) {
    setStripBrightness(strip1, brightness);
    setStripBrightness(strip2, brightness);
    delay(duration / 200); 
  }
  
  for (int brightness = 255; brightness >= 0; brightness--) {
    setStripBrightness(strip1, brightness);
    setStripBrightness(strip2, brightness);
    delay(duration / 200); 
  }
}

void situationGoodToGo() {
  uint32_t green = strip1.Color(0, 255, 0);
  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, green);
    strip2.setPixelColor(i, green);
  }
  strip1.show();
  strip2.show();
  delay(3000);
  strip1.clear();
  strip2.clear();
  strip1.show();
  strip2.show();
}

void situationCalibration() {
  uint32_t mintGreen = strip1.Color(0x3f, 0xa7, 0xd6);
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, mintGreen);
      strip2.setPixelColor(i, mintGreen);
    }
    strip1.show();
    strip2.show();
    delay(500);
    strip1.clear();
    strip2.clear();
    strip1.show();
    strip2.show();
    delay(250);
  }


}

void situationWin() {
  // Gradual rainbow effect
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = strip1.gamma32(strip1.ColorHSV(i * 65536L / NUM_LEDS));
    strip1.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
    strip1.show();
    strip2.show();
    delay(50);
  }
  delay(500);

  // Blinking rainbow effect with skipping LEDs
  for (int j = 0; j < 7; j++) {
    for (int i = 0; i < NUM_LEDS; i += 2) {
      uint32_t color = strip1.gamma32(strip1.ColorHSV(i * 65536L / NUM_LEDS));
      strip1.setPixelColor(i, color);
      strip2.setPixelColor(i, color);
    }
    strip1.show();
    strip2.show();
    delay(100);
    strip1.clear();
    strip2.clear();
    strip1.show();
    strip2.show();
    delay(100);
  }
}


void situationLose() {
  uint32_t red = strip1.Color(255, 0, 0);

  // Sectional lighting
  for (int i = 0; i < 5; i++) {
    strip1.setPixelColor(i, red);
    strip2.setPixelColor(i, red);
  }
  delay(750);
  strip1.show();
  strip2.show();
  

  for (int i = 5; i < 10; i++) {
    strip1.setPixelColor(i, red);
    strip2.setPixelColor(i, red);
  }
  delay(750);
  strip1.show();
  strip2.show();

  for (int i = 10; i < 15; i++) {
    strip1.setPixelColor(i, red);
    strip2.setPixelColor(i, red);
  }

  delay(750);
  strip1.show();
  strip2.show();

  // Blinking all red
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, red);
      strip2.setPixelColor(i, red);
    }
    strip1.show();
    strip2.show();
    delay(250);
    strip1.clear();
    strip2.clear();
    delay(250);
    strip1.show();
    strip2.show();
    delay(250);
  }
}

void situationLevel1() {
  uint32_t green = strip1.Color(0, 255, 0);
  uint32_t yellow = strip1.Color(255, 255, 0);

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      uint32_t color = blend(green, yellow, i, NUM_LEDS);
      strip1.setPixelColor(i, color);
      strip2.setPixelColor(i, color);
      strip1.show();
      strip2.show();
      delay(100);
    }
    strip1.clear();
    strip2.clear();
    strip1.show();
    strip2.show();
  }
}

void situationLevel2() {
  uint32_t greenYellow = strip1.Color(173, 255, 47);
  delay(2000);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, greenYellow);
  }
  strip1.show();
  delay(4000);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip2.setPixelColor(i, greenYellow);
  }
  strip2.show();
  delay(4000);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, greenYellow);
  }
  strip1.show();
  delay(4000);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip2.setPixelColor(i, greenYellow);
  }
  strip2.show();
  delay(4000);
}

uint32_t blend(uint32_t color1, uint32_t color2, uint16_t step, uint16_t totalSteps) {
  uint8_t r1 = (color1 >> 16) & 0xFF;
  uint8_t g1 = (color1 >> 8) & 0xFF;
  uint8_t b1 = color1 & 0xFF;

  uint8_t r2 = (color2 >> 16) & 0xFF;
  uint8_t g2 = (color2 >> 8) & 0xFF;
  uint8_t b2 = color2 & 0xFF;

  uint8_t r = r1 + ((r2 - r1) * step / totalSteps);
  uint8_t g = g1 + ((g2 - g1) * step / totalSteps);
  uint8_t b = b1 + ((b2 - b1) * step / totalSteps);

  return (r << 16) | (g << 8) | b;
}