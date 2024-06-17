#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

Adafruit_MPU6050 mpu;

// Define the number of LEDs in the strip
#define NUM_LEDS 9
#define DATA_PIN 5  // Data pin connected to the DIN of the LED strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause until serial console opens

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate roll angle
  float roll = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;

  // Determine the number of LEDs to light based on the roll angle
  int numLEDsToLight = 0;
  if (roll >= 0 && roll <= 45) {
    numLEDsToLight = map(roll, 0, 45, 0, NUM_LEDS);
  } else if (roll > 45) {
    numLEDsToLight = NUM_LEDS;
  } else {
    numLEDsToLight = 0;
  }

  // Determine the brightness level (0 to 255)
  int brightness = 0;
  if (numLEDsToLight == NUM_LEDS) {
    // Increase brightness when all LEDs are lit
    brightness = map(roll, 45, 60, 255, 255);  // Adjust the roll angle range as needed
    brightness = constrain(brightness, 0, 255);
  } else {
    brightness = 255;  // Standard brightness level
  }

  // Light up the corresponding number of LEDs with the determined brightness
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLEDsToLight) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness)); // Set color to white with variable brightness
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off
    }
  }

  strip.show(); // Update the strip to show the changes

  // Print roll and number of LEDs lit for debugging
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print("\tNumber of LEDs Lit: "); Serial.println(numLEDsToLight);

  delay(100);
}
