#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "FlexSensor.h"
#include "VoltageSensor.h"
#include "Multiplexer.h"
#include "MPUManager.h"
#include "Max30102Manager.h"
#include "LEDControl.h"

const char* ssid = "Dialog 4G 149";
const char* password = "Me1_P@5s";

WebSocketsClient webSocket;

#define LED_STRIP_PIN_1 5
#define LED_STRIP_PIN_2 15
#define NUM_LEDS 15

#define TCA9548A_ADDRESS 0x70 // Default I2C address for the TCA9548A multiplexer

Multiplexer mux(TCA9548A_ADDRESS);
MPUManager mpuCenter;
MPUManager mpuRight;
MPUManager mpuLeft;
Max30102Manager max30102;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to server");
            {
                // Send initial connection status message
                connectionEstablish();
            }
            break;
        case WStype_TEXT:
            Serial.printf("Message from server: %s\n", payload);
            {
              StaticJsonDocument<200> doc;

              // Deserialize the JSON document
              DeserializationError error = deserializeJson(doc, payload);
              if (error) {
                  Serial.print(F("deserializeJson() failed: "));
                  Serial.println(error.f_str());
                  return;
              }

              const char* message = doc["message"];
              
              if (strcmp(message, "body-checking") == 0) {
                  Serial.println("Yes");
                  bodyCheckLEDSetup();
                  for (int i = 0; i < 3; i++)
                    fadeInAndOut(1500);
              } else if(strcmp(message, "false") == 0){
                  connectionEstablish();
              }else{
                Serial.println(message);
              }

            }
            break;
        case WStype_ERROR:
            Serial.println("WebSocket error!");
            break;
        case WStype_BIN:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

void connectionEstablish(){
  String connectionStatus = "{\"ConnectionStatus\": true}";
  webSocket.sendTXT(connectionStatus);
  startLight();
}

void startLight(){ 
  for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0));
      strip2.setPixelColor(i, strip2.Color(0, 0, 0));
  }

  strip1.show();
  strip2.show();

  for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, strip1.Color(0, 255, 0));
      strip2.setPixelColor(i, strip2.Color(0, 255, 0));
  }

  strip1.show();
  strip2.show();
  delay(3000); 

  for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0));
      strip2.setPixelColor(i, strip2.Color(0, 0, 0));
  }
  
  strip1.show();
  strip2.show();

  
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    webSocket.begin("192.168.8.122", 8080, "/");
    webSocket.onEvent(webSocketEvent);

    
    setupVoltageSensor();

    mux.selectChannel(0);
    mpuCenter.initializeMPU();

    mux.selectChannel(1);
    mpuRight.initializeMPU();

    mux.selectChannel(2);
    mpuLeft.initializeMPU(); 

    mux.selectChannel(3);
    max30102.initializeSensor();

    calibrateFlex();

    strip1.begin();
    strip2.begin();

}

void loop() {
    webSocket.loop();
    int batteryPercentage = (int) loopVoltageSensor();

    int flex1Angle = getFlexAngle(FLEX_SENSOR_1_PIN, flex1Initial);
    int flex2Angle = getFlexAngle(FLEX_SENSOR_2_PIN, flex2Initial);

    int anglesCenter[3] = {0, 0, 0};
    int anglesRight[3] = {0, 0, 0};
    int anglesLeft[3] = {0, 0, 0};
    
    // Get data from center MPU
    mux.selectChannel(0);
    int* angles = mpuCenter.getMPUAngles();
    if (angles != nullptr) {
        anglesCenter[0] = angles[0];
        anglesCenter[1] = angles[1];
        anglesCenter[2] = angles[2];
    }

    // Get data from right MPU
    mux.selectChannel(1);
    angles = mpuRight.getMPUAngles();
    if (angles != nullptr) {
        anglesRight[0] = angles[0];
        anglesRight[1] = angles[1];
        anglesRight[2] = angles[2];
    }

    // Get data from left MPU
    mux.selectChannel(2);
    angles = mpuLeft.getMPUAngles();
    if (angles != nullptr) {
        anglesLeft[0] = angles[0];
        anglesLeft[1] = angles[1];
        anglesLeft[2] = angles[2];
    }

    mux.selectChannel(3);
    int bpm = max30102.getBPM();

    String data = "{\"flex1Angle\": " + String(flex1Angle) +
                  ", \"flex2Angle\": " + String(flex2Angle) + 
                  ", \"batteryPercentage\": " + String(batteryPercentage) +
                  ", \"yawCenter\": " + String(anglesCenter[0]) + 
                  ", \"pitchCenter\": " + String(anglesCenter[1]) + 
                  ", \"rollCenter\": " + String(anglesCenter[2]) + 
                  ", \"yawRight\": " + String(anglesRight[0]) + 
                  ", \"pitchRight\": " + String(anglesRight[1]) + 
                  ", \"rollRight\": " + String(anglesRight[2]) + 
                  ", \"yawLeft\": " + String(anglesLeft[0]) + 
                  ", \"pitchLeft\": " + String(anglesLeft[1]) + 
                  ", \"rollLeft\": " + String(anglesLeft[2]) + 
                  ", \"BPM\": " + String(bpm) + "}";

    webSocket.sendTXT(data);
    delay(400);
}
