#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B1 5   // The ESP32 pin GPIO16 connected to the first WS2812B strip
#define PIN_WS2812B2 15  // The ESP32 pin GPIO15 connected to the second WS2812B strip
#define NUM_PIXELS 15    // The number of LEDs (pixels) on each WS2812B LED strip

Adafruit_NeoPixel ws2812b1(NUM_PIXELS, PIN_WS2812B1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ws2812b2(NUM_PIXELS, PIN_WS2812B2, NEO_GRB + NEO_KHZ800);

void setup() {
  ws2812b1.begin();  // initialize first WS2812B strip object (REQUIRED)
  ws2812b2.begin();  // initialize second WS2812B strip object (REQUIRED)
}

void loop() {
  ws2812b1.clear();  // set all pixel colors on the first strip to 'off'
  ws2812b2.clear();  // set all pixel colors on the second strip to 'off'

  // Turn pixels to green on the first strip and blue on the second strip one-by-one with delay between each pixel
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    ws2812b1.setPixelColor(pixel, ws2812b1.Color(0, 255, 0));  // Set pixel to green on the first strip
    ws2812b2.setPixelColor(pixel, ws2812b2.Color(0, 0, 255));  // Set pixel to blue on the second strip
    ws2812b1.show();  // Update the first WS2812B Led Strip
    ws2812b2.show();  // Update the second WS2812B Led Strip
    delay(500);  // 500ms pause between each pixel
  }

  // Turn off all pixels for two seconds
  ws2812b1.clear();
  ws2812b2.clear();
  ws2812b1.show();  // Update the first WS2812B Led Strip
  ws2812b2.show();  // Update the second WS2812B Led Strip
  delay(2000);  // 2 seconds off time

  // Turn on all pixels to red on the first strip and yellow on the second strip at the same time for two seconds
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    ws2812b1.setPixelColor(pixel, ws2812b1ic.Color(255, 0, 0));  // Set pixel to red on the first strip
    ws2812b2.setPixelColor(pixel, ws2812b2.Color(255, 255, 0));  // Set pixel to yellow on the second strip
  }
  ws2812b1.show();  // Update the first WS2812B Led Strip
  ws2812b2.show();  // Update the second WS2812B Led Strip
  delay(1000);  // 1 second on time

  // Turn off all pixels for one second
  ws2812b1.clear();
  ws2812b2.clear();
  ws2812b1.show();  // Update the first WS2812B Led Strip
  ws2812b2.show();  // Update the second WS2812B Led Strip
  delay(1000);  // 1 second off time
}
