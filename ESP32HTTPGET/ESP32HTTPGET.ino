// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "underwaterbot";

AsyncWebServer server(80);

// [For potentiometer]
int potpin = 34;
int potvalue;
int realpotvalue;

// [For Joystick]
int Vx = 14;
int xVal;
int centerx;
float xplusgrad;
float xplusconstant;
int xplus;
int xminus;
int x;
float xminusgrad;

int Vy = 12;
int yVal;
int centery;
float yplusgrad;
int yplus;
int yminus;
int y;
float yminusgrad;
float yplusconstant;

// [For button]
int buttpin = 13;
int buttstate;
int button = 0;
String controls;
char controlChars[20];

void setup() {
  // put your setup code here, to run once:
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/potentiometer", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send_P(200, "text/plain", controlChars);
  });
    Serial.println("Network connection works");   
  
  // Start server
  server.begin();
  
//joystick
centerx = analogRead(Vx);
xplusgrad = 512.0/(4095.0 - centerx);
xplusconstant = 1023 - xplusgrad*4095;
xminusgrad = 512/centerx;

centery = analogRead(Vy);
yplusgrad = 512.0/(4095.0 - centery);
yminusgrad = 512/centery;
yplusconstant = 1023 - yplusgrad*4095;
delay(500);

}
void loop() {
  // For potentiometer
 potvalue = analogRead(potpin);   // potvalue will used to determine motor speed
 realpotvalue = map(potvalue, 0, 4095, 0, 1023);
 Serial.println(realpotvalue);

 // For Joystick
 int x = analogRead(Vx);
 int y = analogRead(Vy);


xplus = xplusgrad*x + xplusconstant;
xminus = xminusgrad*x;
if (x > 512) {
  Serial.print("x ");
  Serial.print(xplus);
} else {
  Serial.print("x ");
  Serial.print(xminus);
}

yplus = yplusgrad*y + yplusconstant;
yminus = yminusgrad*y;
if (y > 512) {
  Serial.print(" y ");
  Serial.print(yplus);
} else {
  Serial.print(" y ");
  Serial.print(yminus);
 
}
Serial.println();

 // For button
 buttstate = digitalRead(buttpin);
 if (buttstate == 1){
    button++;
    Serial.println(button);
    delay(500);
  } else{
    Serial.println(button);
    }

 controls = String(realpotvalue) + " " + String(button) + " " + String(x) + " " + String(y);
 controls.toCharArray(controlChars, 20);

}
