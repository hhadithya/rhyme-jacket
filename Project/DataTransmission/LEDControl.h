#include <Adafruit_NeoPixel.h>

// Define the number of LEDs in each strip
#define NUM_LEDS 15

// Define the pins where the LED strips are connected
#define LED_PIN_1 5
#define LED_PIN_2 15

// Create two Adafruit_NeoPixel objects, one for each strip
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, LED_PIN_2, NEO_GRB + NEO_KHZ800);

void bodyCheckLEDSetup() {
  // Initialize both strips
  strip1.begin();
  strip2.begin();

  // Set brightness for both strips (0 to 255)
  strip1.setBrightness(255);
  strip2.setBrightness(255);

  // Show initial state
  strip1.show();
  strip2.show();
}

void setStripBrightness(Adafruit_NeoPixel& strip, int brightness) {
  // Set the color to a bluish hue, adjusting brightness for the blue component
  int blueValue = brightness; // Full blue
  int redValue = brightness / 4; // Less red for a bluish hue
  int greenValue = brightness / 8; // Even less green for a bluish hue

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue)); // Bluish color
  }
  strip.show();
}

void fadeInAndOut(uint16_t duration) {
  for (int brightness = 0; brightness <= 255; brightness++) {
    setStripBrightness(strip1, brightness);
    setStripBrightness(strip2, brightness);
    delay(duration / 200); // Adjust delay for smooth transition
  }
  
  for (int brightness = 255; brightness >= 0; brightness--) {
    setStripBrightness(strip1, brightness);
    setStripBrightness(strip2, brightness);
    delay(duration / 200); // Adjust delay for smooth transition
  }
}
