#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <BleMouse.h>
#include "FlexSensor.h"
#include "VoltageSensor.h"
#include "Multiplexer.h"
#include "MPUManager.h"
#include "LEDControl.h"
#include "TemperatureSensor.h"
#include "HeartRateSensor.h"


// Wifi Credentials
const char* ssid = "Dialog 4G 149";
const char* password = "Me1_P@5s";

int i = 0;

BleMouse bleMouse;
WebSocketsClient webSocket;

#define LED_STRIP_PIN_1 5
#define LED_STRIP_PIN_2 15
#define NUM_LEDS 15

#define TCA9548A_ADDRESS 0x70 // Default I2C address for the TCA9548A multiplexer

Multiplexer mux(TCA9548A_ADDRESS);
MPUManager mpuCenter;
MPUManager mpuRight;
MPUManager mpuLeft;
HeartRateSensor heartRateSensor(3, mux);

bool shouldSendData = true;
bool mouseStatus = true;
bool gameActive = false;
bool statsActive = false;
int bpm, filteredBPM, mpuCenterStatus, mpuLeftStatus, mpuRightStatus;
float avgTemp;
String mpuStatusData;

void connectionEstablish(){
  String connectionStatus = "{\"connection_status\": true}";
  webSocket.sendTXT(connectionStatus);
  situationGoodToGo();
}

void calibrateSensors(){
    delay(1000);
    mux.selectChannel(0);
    do{
      mpuCenterStatus = mpuCenter.initializeMPU(true);
    }while (!mpuCenterStatus);

    situationCalibration();
    delay(4000);

    mux.selectChannel(1);
    do{
      mpuRightStatus = mpuRight.initializeMPU(false);
    }while (!mpuRightStatus);

    situationCalibration();

    mux.selectChannel(2);
    do{
      mpuLeftStatus = mpuLeft.initializeMPU(false); 
    }while (!mpuLeftStatus);

    situationCalibration();

    calibrateFlex();

    situationCalibration();
    String data = "{\"calibration_status\": \"calibration_done\"}";
    webSocket.sendTXT(data);

  uint32_t mintGreen = strip1.Color(0x3f, 0xa7, 0xd6);
  for (int i = 0; i < NUM_LEDS; i++) {
      strip1.setPixelColor(i, mintGreen);
      strip2.setPixelColor(i, mintGreen);
  }
  strip1.show();
  strip2.show();
  delay(3000);
  strip1.clear();
  strip2.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, 0,0,0);
    strip2.setPixelColor(i, 0,0,0);
  }
  strip1.show();
  strip2.show();
}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to server");
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
              
              if (strcmp(message, "connecting") == 0) {
                delay(2000);
                connectionEstablish();
              } else if (strcmp(message, "stop") == 0) {
                shouldSendData = false;
                Serial.println("Stopping data transmission");
              } else if (strcmp(message, "body_checking") == 0) {
                batteryLevel();
                avgTemp = tempLoop("body_check");
                Serial.println(bpm);
                String data = "{\"bpm\": " + String(bpm) + ", \"temperature\": " + String(avgTemp) + "}";
                webSocket.sendTXT(data);
              } else if(strcmp(message, "l1_calibration") == 0 || strcmp(message, "stat_calibration") == 0 || strcmp(message, "l2_calibration") == 0){
                calibrateSensors();
              } else if(strcmp(message, "l1_game") == 0 || strcmp(message, "l2_game") == 0){
                batteryLevel();
                delay(4000);
                gameActive = true;
                
                mouseStatus = false;
              } else if(strcmp(message, "quit") == 0){
                gameActive = false;
                mouseStatus = true;
              } else if(strcmp(message, "sensor_readings") == 0){
                batteryLevel();
                statsActive = true;
                mouseStatus = false;
              }else if(strcmp(message, "l1_win") == 0){
                batteryLevel();
                situationWin();
              } else if(strcmp(message, "l1_lose") == 0){
                batteryLevel();
                situationLose();
              } else{
                statsActive = false;
                batteryLevel();
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


void setup() {
    Serial.begin(115200);
    bleMouse.begin();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    webSocket.begin("192.168.8.112", 8080, "/");
    webSocket.onEvent(webSocketEvent);
    
    setupVoltageSensor();

    mux.selectChannel(1);
    mpuRightStatus = mpuRight.initializeMPU(false);

    do{
      mpuRightStatus = mpuRight.initializeMPU(false);
    }while (!mpuRightStatus);

    if (heartRateSensor.begin()) {
        Serial.println("Heart Rate Sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize Heart Rate Sensor.");
    }

    setupLEDs();

    pinMode(18, INPUT_PULLUP);
}

void loop() {
    webSocket.loop();
    
    if (shouldSendData) {
        heartRateSensor.update();
        static unsigned long lastSendTime = 0;
        if (millis() - lastSendTime >= 1000) {
            lastSendTime = millis();
            filteredBPM = (int)heartRateSensor.getFilteredBPM(); // Convert BPM to integer
            bpm = filteredBPM;
            String message = "{\"type\": \"bpm\", \"value\" : " + String(filteredBPM) + "}";
            webSocket.sendTXT(message);
        }
    }
    if (!mouseStatus && digitalRead(18) == LOW){
      mouseStatus = true;
      statsActive = false;
    }

    if (mouseStatus){
      mux.selectChannel(1);
      int gyroX, gyroY, gyroZ;
      mpuRight.getRawGyroValues(gyroX, gyroY, gyroZ);
      gyroX = gyroX / 1.7;
      gyroY = gyroY/ 1.2;

      if (bleMouse.isConnected()) {
        bleMouse.move(gyroX, gyroY);
        if (digitalRead(18) == LOW) {
          bleMouse.click(MOUSE_LEFT);
          delay(250);
        }
      }
    }

    if (gameActive) {
      sensorDataLoop();
    }

    if (statsActive){
      statsData();
    }

    delay(10);
}

void sensorDataLoop() {
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

    String data = "{\"flex1Angle\": " + String(flex1Angle) +
                  ", \"flex2Angle\": " + String(flex2Angle) + 
                  ", \"yawCenter\": " + String(anglesCenter[0]) + 
                  ", \"pitchCenter\": " + String(anglesCenter[1]) + 
                  ", \"rollCenter\": " + String(anglesCenter[2]) + 
                  ", \"yawRight\": " + String(anglesRight[0]) + 
                  ", \"pitchRight\": " + String(anglesRight[1]) + 
                  ", \"rollRight\": " + String(anglesRight[2]) + 
                  ", \"yawLeft\": " + String(anglesLeft[0]) + 
                  ", \"pitchLeft\": " + String(anglesLeft[1]) + 
                  ", \"rollLeft\": " + String(anglesLeft[2]) + "}";

    webSocket.sendTXT(data);
    delay(100);
}


void statsData(){
  if (i == 1000){
    i = 0;
  }
  int flex1Angle = getFlexAngle(FLEX_SENSOR_1_PIN, flex1Initial);
  int flex2Angle = getFlexAngle(FLEX_SENSOR_2_PIN, flex2Initial);

  if (i == 0){
    heartRateSensor.update();
    filteredBPM = heartRateSensor.getFilteredBPM();
    avgTemp = tempLoop("none");
  }
  i = i + 1;
  

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

  String data = "{\"flex1Angle\": " + String(flex1Angle) +
                ", \"flex2Angle\": " + String(flex2Angle) +
                ", \"yawCenter\": " + String(anglesCenter[0]) + 
                ", \"pitchCenter\": " + String(anglesCenter[1]) + 
                ", \"rollCenter\": " + String(anglesCenter[2]) + 
                ", \"yawRight\": " + String(anglesRight[0]) + 
                ", \"pitchRight\": " + String(anglesRight[1]) + 
                ", \"rollRight\": " + String(anglesRight[2]) + 
                ", \"yawLeft\": " + String(anglesLeft[0]) + 
                ", \"pitchLeft\": " + String(anglesLeft[1]) + 
                ", \"rollLeft\": " + String(anglesLeft[2]) +
                ", \"bpm\": " + String(filteredBPM) + 
                ", \"temperature\": " + String(avgTemp) +"}";

  webSocket.sendTXT(data);
  delay(100);
}

void batteryLevel(){
  int batteryPercentage = (int) loopVoltageSensor();
  String data = "{\"batteryPercentage\": " + String(batteryPercentage) + "}";
  webSocket.sendTXT(data);
}
