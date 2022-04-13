// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "underwaterbot";

AsyncWebServer server(80);

// [For potentiometer]
int potpin = 34 ;
int potvalue;
int realpot;
int potfinal;

// [For Joystick]
int Vx = 35;
int centrex;
float xplusgrad;
float xplusconstant;
int xplus;
int xminus;
int x;
float xminusgrad;
int finalx;

int Vy = 32;
int centrey;
float yplusgrad;
int yplus;
int yminus;
int y;
float yminusgrad;
float yplusconstant;
int finaly;

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

centrex = analogRead(Vx);
xplusgrad = 511.0/(4095.0 - centrex);
xplusconstant = 1023.0 - xplusgrad*4095.0;
xminusgrad = 512.0/centrex;


centrey = analogRead(Vy);
yplusgrad = 511.0/(4095.0 - centrey);
yminusgrad = 512.0/centrey;
yplusconstant = 1023.0 - yplusgrad*4095.0;


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
  
}
void loop() {
 // For potentiometer
 potvalue = analogRead(potpin);
 realpot = map(potvalue, 0, 4095, 0, 1023);
 potfinal = 1023 - realpot;
 //Serial.println(String(potvalue)+" "+String(potfinal));
 
 // For Joystick
 x = analogRead(Vx);
 y = analogRead(Vy);

xplus = xplusgrad*x + xplusconstant;
xminus = xminusgrad*x;
if (x >= centrex) {
  finalx = xplus;
} else {
  finalx = xminus;
}
Serial.print("x ");
Serial.print(finalx);

yplus = yplusgrad*y + yplusconstant;
yminus = yminusgrad*y;
if (y >= centrey) {
  finaly = yplus;
} else {
  finaly = yminus;
}
Serial.print("  y ");
Serial.println(finaly);


 // For button
 buttstate = digitalRead(buttpin);

  if (buttstate == 1) {
    button++;
if (button==2){
button = 0;
}
    while (buttstate == 1)
    {
     buttstate = digitalRead(buttpin);
      }

  } 
  else {
  }

 controls = String(potfinal) + " " + String(button) + " " + String(finalx) + " " + String(finaly);
 controls.toCharArray(controlChars, 20);
}
