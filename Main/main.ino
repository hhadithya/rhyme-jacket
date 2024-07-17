#include <Wire.h>
#include "FlexSensor.h"
#include "HeartRateSensor.h"
#include "MPU6050Sensor.h"
#include "WebSocketClient.h"
#include "TemperatureSensor.h"

const char* ssid = "Galaxy A520F4D";
const char* password = "sani1088";
const char* websocket_server = "192.168.157.103";  // Replace with your server's IP address
const uint16_t websocket_port = 3000;

void setup() {
  Serial.begin(115200);

  // Setup sensors
  calibrateFlex();
  // setupHeartRateSensor();
  setupMPU6050();

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup WebSocket
  setupWebSocket(websocket_server, websocket_port);
}

void loop() {
  // Get sensor data
  int flex1Angle = getFlexAngle(FLEX_SENSOR_1_PIN, flex1Initial);
  int flex2Angle = getFlexAngle(FLEX_SENSOR_2_PIN, flex2Initial);
  // int heartRate = getHeartRate();
  readMPU6050();

  // Create JSON string with sensor data
  String json = "{\"flex1Angle\":" + String(flex1Angle) + ",\"flex2Angle\":" + String(flex2Angle);
  // if (heartRate != -1) {
  //   json += ",\"heartRate\":" + String(heartRate);
  // }
  json += ",\"rollAngle\":" + String(AngleRoll) + ",\"pitchAngle\":" + String(AnglePitch) + "}";

  // Send data to WebSocket server
  sendWebSocketData(json.c_str());

  // Handle WebSocket events
  handleWebSocket();

  delay(500); // Delay for readability
}