#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// =====================================================
// ESP32 SMART WALL SWITCH
// GitHub Project Version
// =====================================================

// ================= WIFI CONFIG =================

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ================= SERVO OBJECTS =================

Servo servoON;
Servo servoOFF;

// ================= SERVO PINS =================

const int servoON_pin  = 13;
const int servoOFF_pin = 12;

// ================= SERVO ANGLES =================

// ON servo
int servoON_center = 90;
int servoON_push   = 127;

// OFF servo
int servoOFF_push   = 61;
int servoOFF_center = 117;

// ================= WEB SERVER =================

WebServer server(80);

// =====================================================
// FUNCTION: TURN LIGHT ON
// =====================================================

void turnON() {

  servoON.write(servoON_push);

  delay(700);

  servoON.write(servoON_center);

}

// =====================================================
// FUNCTION: TURN LIGHT OFF
// =====================================================

void turnOFF() {

  servoOFF.write(servoOFF_push);

  delay(700);

  servoOFF.write(servoOFF_center);

}

// =====================================================
// WEBPAGE HTML
// =====================================================

String webpage = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<title>ESP32 Smart Switch</title>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
    font-family: Arial;
    text-align:center;
    background:#111;
    color:white;
    margin-top:50px;
}

h1{
    margin-bottom:40px;
}

button{
    width:220px;
    height:90px;
    font-size:30px;
    margin:20px;
    border:none;
    border-radius:20px;
    cursor:pointer;
}

.on{
    background:#00cc66;
    color:white;
}

.off{
    background:#ff4444;
    color:white;
}

</style>

</head>

<body>

<h1>ESP32 SMART WALL SWITCH</h1>

<button class="on" onclick="location.href='/on'">
ON
</button>

<br>

<button class="off" onclick="location.href='/off'">
OFF
</button>

</body>
</html>

)rawliteral";

// =====================================================
// HANDLE ROOT PAGE
// =====================================================

void handleRoot() {

  server.send(200, "text/html", webpage);

}

// =====================================================
// HANDLE ON REQUEST
// =====================================================

void handleON() {

  turnON();

  server.sendHeader("Location", "/");

  server.send(303);

}

// =====================================================
// HANDLE OFF REQUEST
// =====================================================

void handleOFF() {

  turnOFF();

  server.sendHeader("Location", "/");

  server.send(303);

}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Allocate ESP32 PWM timers

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  // Servo frequency

  servoON.setPeriodHertz(50);
  servoOFF.setPeriodHertz(50);

  // Attach servos

  servoON.attach(servoON_pin, 500, 2400);
  servoOFF.attach(servoOFF_pin, 500, 2400);

  // Initial servo positions

  servoON.write(servoON_center);
  servoOFF.write(servoOFF_center);

  // Connect to WiFi

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();

  Serial.println("WiFi Connected");

  Serial.print("ESP32 IP Address: ");

  Serial.println(WiFi.localIP());

  // Wait before accepting commands

  delay(5000);

  // Web routes

  server.on("/", handleRoot);

  server.on("/on", handleON);

  server.on("/off", handleOFF);

  // Start server

  server.begin();

  Serial.println("Web Server Started");

}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  server.handleClient();

}