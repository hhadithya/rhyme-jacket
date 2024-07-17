#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define NUM_LEDS 15

// Define the pins for each LED strip
#define LED_STRIP_1_PIN 5
#define LED_STRIP_2_PIN 15

const char* ssid = "Dialog 4G 149";
const char* password = "Me1_P@5s";

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Create Adafruit_NeoPixel objects for each strip
Adafruit_NeoPixel strip1(NUM_LEDS, LED_STRIP_1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUM_LEDS, LED_STRIP_2_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_MPU6050 mpu1, mpu2, mpu3;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 MPU6050</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; }
    #cube-container1, #cube-container2, #cube-container3 { width: 200px; height: 200px; margin: 100px auto; position: relative; display: inline-block; }
    #cube1, #cube2, #cube3 {
      width: 100px;
      height: 100px;
      position: absolute;
      transform-style: preserve-3d;
      transform: scale3d(1, 1, 1);
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
      var data = event.data.split('|');
      var angles1 = data[0].split(',');
      var angles2 = data[1].split(',');
      var angles3 = data[2].split(',');
      var flexValues = data[3].split(',');
      var irValue = parseFloat(data[4]);

      var pitch1 = parseFloat(angles1[0]);
      var roll1 = parseFloat(angles1[1]);
      var yaw1 = parseFloat(angles1[2]);
      var pitch2 = parseFloat(angles2[0]);
      var roll2 = parseFloat(angles2[1]);
      var yaw2 = parseFloat(angles2[2]);
      var pitch3 = parseFloat(angles3[0]);
      var roll3 = parseFloat(angles3[1]);
      var yaw3 = parseFloat(angles3[2]);

      var flex1 = parseFloat(flexValues[0]);
      var flex2 = parseFloat(flexValues[1]);

      document.getElementById('pitch1').innerText = 'Pitch1: ' + pitch1.toFixed(2);
      document.getElementById('roll1').innerText = 'Roll1: ' + roll1.toFixed(2);
      document.getElementById('yaw1').innerText = 'Yaw1: ' + yaw1.toFixed(2);
      document.getElementById('pitch2').innerText = 'Pitch2: ' + pitch2.toFixed(2);
      document.getElementById('roll2').innerText = 'Roll2: ' + roll2.toFixed(2);
      document.getElementById('yaw2').innerText = 'Yaw2: ' + yaw2.toFixed(2);
      document.getElementById('pitch3').innerText = 'Pitch3: ' + pitch3.toFixed(2);
      document.getElementById('roll3').innerText = 'Roll3: ' + roll3.toFixed(2);
      document.getElementById('yaw3').innerText = 'Yaw3: ' + yaw3.toFixed(2);
      
      document.getElementById('flex1').innerText = 'Flex1: ' + flex1.toFixed(2);
      document.getElementById('flex2').innerText = 'Flex2: ' + flex2.toFixed(2);
      document.getElementById('bpm').innerText = 'Heart Rate: ' + irValue.toFixed(2);
      document.getElementById('Temp').innerText = 'Temperature: ' + flex2.toFixed(2);

      document.getElementById('cube1').style.transform = 'rotateX(' + pitch1 + 'deg) rotateY(' + roll1 + 'deg) rotateZ(' + yaw1 + 'deg)';
      document.getElementById('cube2').style.transform = 'rotateX(' + pitch2 + 'deg) rotateY(' + roll2 + 'deg) rotateZ(' + yaw2 + 'deg)';
      document.getElementById('cube3').style.transform = 'rotateX(' + pitch3 + 'deg) rotateY(' + roll3 + 'deg) rotateZ(' + yaw3 + 'deg)';

    };
  </script>
</head>
<body>
  <h1>All the Data</h1>
  <div style="display: flex; flex-direction: column; width: 100vw; justify-content: space-around;">
    <div style="display: flex; width: 100vw; justify-content: space-around;">
      <div>
        <div>
          <p id="pitch1">Pitch1: </p>
          <p id="roll1">Roll1: </p>
          <p id="yaw1">Yaw1: </p>
        </div>
        <div id="cube-container1">
          <div id="cube1">
            <div class="face front"></div>
            <div class="face back"></div>
            <div class="face right"></div>
            <div class="face left"></div>
            <div class="face top"></div>
            <div class="face bottom"></div>
          </div>
        </div>
      </div>
      <div>
        <div>
          <p id="pitch2">Pitch2: </p>
          <p id="roll2">Roll2: </p>
          <p id="yaw2">Yaw2: </p>
        </div>
        <div id="cube-container2">
          <div id="cube2">
            <div class="face front"></div>
            <div class="face back"></div>
            <div class="face right"></div>
            <div class="face left"></div>
            <div class="face top"></div>
            <div class="face bottom"></div>
          </div>
        </div>
      </div>
      <div>
        <div>
          <p id="pitch3">Pitch3: </p>
          <p id="roll3">Roll3: </p>
          <p id="yaw3">Yaw3: </p>
        </div>
        <div id="cube-container3">
          <div id="cube3">
            <div class="face front"></div>
            <div class="face back"></div>
            <div class="face right"></div>
            <div class="face left"></div>
            <div class="face top"></div>
            <div class="face bottom"></div>
          </div>
        </div>
      </div>
    </div>
    <div>
      <div>
        <p id="flex1">Flex1: </p>
        <p id="flex2">Flex2: </p>
        <p id="bpm">Heart Rate: </p>
        <p id="Temp">Temperature: </p>
      </div>
    </div> 
  </div>
</body>
</html>
)rawliteral";


void selectChannel(uint8_t channel) {
  Wire.beginTransmission(0x70); // TCA9548A address is 0x70
  Wire.write(1 << channel);
  Wire.endTransmission();
}

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
  Wire.begin();

  strip1.begin(); // Initialize the first LED strip
  strip2.begin(); // Initialize the second LED strip
  strip1.show();  // Initialize all pixels to 'off'
  strip2.show();  // Initialize all pixels to 'off'
  
  pinMode(34, INPUT);
  pinMode(35, INPUT);

  selectChannel(0);
  if (!max30102.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("Failed to find MAX30102 chip on channel 0");
    while (1) { delay(10); }
  }
  max30102.setup(); // Configure sensor with default settings

  // Initialize MPU6050 sensors through the multiplexer
  selectChannel(1);
  if (!mpu1.begin()) {
    Serial.println("Failed to find MPU6050 chip on channel 1");
    while (1) { delay(10); }
  }

  selectChannel(2);
  if (!mpu2.begin()) {
    Serial.println("Failed to find MPU6050 chip on channel 2");
    while (1) { delay(10); }
  }

  selectChannel(3);
  if (!mpu3.begin()) {
    Serial.println("Failed to find MPU6050 chip on channel 3");
    while (1) { delay(10); }
  }

  mpu1.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu1.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu1.setFilterBandwidth(MPU6050_BAND_21_HZ);

  mpu2.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu2.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu2.setFilterBandwidth(MPU6050_BAND_21_HZ);

  mpu3.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu3.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu3.setFilterBandwidth(MPU6050_BAND_21_HZ);

  initWiFi();
  initWebSocket();
  initWebServer();
}


void loop() {
  handleWebSocket();

  // Read data from MAX30102
  selectChannel(0);
  long irValue = max30102.getIR(); // Get IR value from the sensor

  // Read data from MPU1
  selectChannel(1);
  sensors_event_t a1, g1, temp1;
  mpu1.getEvent(&a1, &g1, &temp1);

  float accX1 = a1.acceleration.x;
  float accY1 = a1.acceleration.y;
  float accZ1 = a1.acceleration.z;

  float gyroX1 = g1.gyro.x;
  float gyroY1 = g1.gyro.y;
  float gyroZ1 = g1.gyro.z;

  float pitch1 = atan2(accY1, sqrt(accX1 * accX1 + accZ1 * accZ1)) * 180 / PI;
  float roll1 = atan2(-accX1, accZ1) * 180 / PI;
  float yaw1 = gyroZ1; // Simplified

  // Read data from MPU2
  selectChannel(2);
  sensors_event_t a2, g2, temp2;
  mpu2.getEvent(&a2, &g2, &temp2);

  float accX2 = a2.acceleration.x;
  float accY2 = a2.acceleration.y;
  float accZ2 = a2.acceleration.z;

  float gyroX2 = g2.gyro.x;
  float gyroY2 = g2.gyro.y;
  float gyroZ2 = g2.gyro.z;

  float pitch2 = atan2(accY2, sqrt(accX2 * accX2 + accZ2 * accZ2)) * 180 / PI;
  float roll2 = atan2(-accX2, accZ2) * 180 / PI;
  float yaw2 = gyroZ2; // Simplified

  // Read data from MPU3
  selectChannel(3);
  sensors_event_t a3, g3, temp3;
  mpu3.getEvent(&a3, &g3, &temp3);

  float accX3 = a3.acceleration.x;
  float accY3 = a3.acceleration.y;
  float accZ3 = a3.acceleration.z;

  float gyroX3 = g3.gyro.x;
  float gyroY3 = g3.gyro.y;
  float gyroZ3 = g3.gyro.z;

  float pitch3 = atan2(accY3, sqrt(accX3 * accX3 + accZ3 * accZ3)) * 180 / PI;
  float roll3 = atan2(-accX3, accZ3) * 180 / PI;
  float yaw3 = gyroZ3; // Simplified

  // Read data from Flex Sensors
  int flex1 = analogRead(34);
  int flex2 = analogRead(35);

  // Control the WS2812 LED strips
  int numLeds1 = map(roll1, 0, 45, 0, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLeds1) {
      strip1.setPixelColor(i, strip1.Color(0, 0, 255)); // Blue color
    } else {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0)); // Turn off the LED
    }
  }
  strip1.show();

  int numLeds2 = map(roll2, 0, 45, 0, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLeds2) {
      strip2.setPixelColor(i, strip2.Color(128, 0, 128)); // Purple color
    } else {
      strip2.setPixelColor(i, strip2.Color(0, 0, 0)); // Turn off the LED
    }
  }
  strip2.show();

  // Send data to the web client
  String angleData = String(pitch1) + "," + String(roll1) + "," + String(yaw1) + "|" + 
                     String(pitch2) + "," + String(roll2) + "," + String(yaw2) + "|" +
                     String(pitch3) + "," + String(roll3) + "," + String(yaw3) + "|" +
                     String(flex1) + "," + String(flex2) + "|" +
                     String(irValue);
  webSocket.broadcastTXT(angleData);

  delay(100);
}
