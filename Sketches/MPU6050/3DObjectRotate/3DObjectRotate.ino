#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

Adafruit_MPU6050 mpu;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 MPU6050</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; perspective: 1000px; }
    #cube-container { width: 200px; height: 200px; margin: 100px auto; position: relative; }
    #cube {
      width: 100px;
      height: 100px;
      position: absolute;
      transform-style: preserve-3d;
      transform: rotateX(0deg) rotateY(0deg) rotateZ(0deg);
      transition: transform 0.1s;
    }
    .face {
      position: absolute;
      width: 100px;
      height: 100px;
      opacity: 0.8;
      border: 1px solid #000;
    }
    .front  { background: red;    transform: translateZ(50px); }
    .back   { background: yellow; transform: rotateY(180deg) translateZ(50px); }
    .right  { background: green;  transform: rotateY( 90deg) translateZ(50px); }
    .left   { background: blue;   transform: rotateY(-90deg) translateZ(50px); }
    .top    { background: cyan;   transform: rotateX( 90deg) translateZ(50px); }
    .bottom { background: purple; transform: rotateX(-90deg) translateZ(50px); }
  </style>
  <script>
    var connection = new WebSocket('ws://' + location.hostname + ':81/');
    connection.onmessage = function (event) {
      var angles = event.data.split(',');
      var pitch = parseFloat(angles[0]);
      var roll = parseFloat(angles[1]);
      var yaw = parseFloat(angles[2]);
      document.getElementById('pitch').innerText = 'Pitch: ' + pitch.toFixed(2);
      document.getElementById('roll').innerText = 'Roll: ' + roll.toFixed(2);
      document.getElementById('yaw').innerText = 'Yaw: ' + yaw.toFixed(2);
      document.getElementById('cube').style.transform = 'rotateX(' + pitch + 'deg) rotateY(' + roll + 'deg) rotateZ(' + yaw + 'deg)';
    };
  </script>
</head>
<body>
  <h1>ESP32 MPU6050</h1>
  <p id="pitch">Pitch: </p>
  <p id="roll">Roll: </p>
  <p id="yaw">Yaw: </p>
  <div id="cube-container">
    <div id="cube">
      <div class="face front"></div>
      <div class="face back"></div>
      <div class="face right"></div>
      <div class="face left"></div>
      <div class="face top"></div>
      <div class="face bottom"></div>
    </div>
  </div>
</body>
</html>
)rawliteral";

void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void initWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.begin();
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Handle WebSocket events here
}

void initWebSocket() {
  webSocket.onEvent(onWebSocketEvent);
  webSocket.begin();
}

void handleWebSocket() {
  webSocket.loop();
}

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  initWiFi();
  initWebSocket();
  initWebServer();
}

void loop() {
  handleWebSocket();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accX = a.acceleration.x;
  float accY = a.acceleration.y;
  float accZ = a.acceleration.z;

  float gyroX = g.gyro.x;
  float gyroY = g.gyro.y;
  float gyroZ = g.gyro.z;

  // Calculate pitch, roll, and yaw
  float pitch = atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
  float roll = atan2(-accX, accZ) * 180 / PI;
  float yaw = gyroZ; // This is a simplification. In reality, yaw is more complex to calculate.

  String angleData = String(pitch) + "," + String(roll) + "," + String(yaw);
  webSocket.broadcastTXT(angleData);

  delay(100);
}
