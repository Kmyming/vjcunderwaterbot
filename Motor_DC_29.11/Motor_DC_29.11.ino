//WiFi
#include <WiFi.h>
#include <HTTPClient.h>
//include the libraries we need

const char* ssid = "ESP32-Access-Point";
const char* password = "underwaterbot";
// setting the ssid and password as variable to be used to connect to the network

unsigned long previousMillis = 0;
const long interval = 5000;
//variables we will be using the delay the POST requests

const char* serverNamepot = "http://192.168.4.1/potentiometer";
String getRequest;
String xvalue;
String yvalue;
String buttvalue;
String potvalue;
// setting up variables we will use

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  //Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

// Motors and electro magnet
int motor1Pin1 = 13; //HRight motor forward
int motor1Pin2 = 12; //HRight motor back
int motor2Pin1 = 27; //HLeft motor forward
int motor2Pin2 = 26; //HLeft motor back
int motor3Pin1 = 25; //VRight motor forward
int motor3Pin2 = 33; //VRight motor back
int motor4Pin1 = 32; //VLeft motor forward
int motor4Pin2 = 35; //VLeft motor back
int magnet = 14; //Electromagnet pin
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmChannel3 = 2;
const int pwmChannel4 = 3;
const int pwmChannel5 = 4;
const int pwmChannel6 = 5;
const int pwmChannel7 = 6;
const int pwmChannel8 = 7;
const int resolution = 10;
int DutyCycleL = 0; //Range of 0 to 1023, Left
int DutyCycleR = 0; //Range of 0 to 1023, Right
int DutyCycleU = 0; //Range of 0 to 1023, Up

float X, Z, Left, Right;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //WiFi
  //connecting to wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for connection...");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address:");
  Serial.println(WiFi.localIP());

  // Motors and electro magnet
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  pinMode(magnet, OUTPUT);
  ledcSetup(pwmChannel1, freq, resolution);
  ledcAttachPin(motor1Pin1, pwmChannel1);

  ledcSetup(pwmChannel2, freq, resolution);
  ledcAttachPin(motor1Pin2, pwmChannel2);

  ledcSetup(pwmChannel3, freq, resolution);
  ledcAttachPin(motor2Pin1, pwmChannel3);

  ledcSetup(pwmChannel4, freq, resolution);
  ledcAttachPin(motor2Pin2, pwmChannel4);

  ledcSetup(pwmChannel5, freq, resolution);
  ledcAttachPin(motor3Pin1, pwmChannel5);

  ledcSetup(pwmChannel6, freq, resolution);
  ledcAttachPin(motor3Pin2, pwmChannel6);

  ledcSetup(pwmChannel7, freq, resolution);
  ledcAttachPin(motor4Pin1, pwmChannel7);

  ledcSetup(pwmChannel8, freq, resolution);
  ledcAttachPin(motor4Pin2, pwmChannel8);

}

void loop() {
  // Put your main code here, to run repeatedly:
  // Insert the recieving code here
  unsigned long currentMillis = millis();
  //check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    getRequest = httpGETRequest(serverNamepot);
    xvalue = getValue(getRequest, ' ', 3);
    yvalue = getValue(getRequest, ' ', 2);
    buttvalue = getValue(getRequest, ' ', 1);
    potvalue = getValue(getRequest, ' ', 0);
    /*   xvalue.toInt();
       yvalue.toInt();
       buttvalue.toInt();
       potvalue.toInt();
      Serial.print ("x ");
      Serial.print(xvalue);
      Serial.print(" y ");
      Serial.print(yvalue);
      Serial.println();
      Serial.println(buttvalue);
      Serial.println(potvalue);*/
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  // Should receive 2 integers for joystick, 1 integer for electromagnet, 1 integer for depth
  xzmovement(xvalue.toFloat(), yvalue.toFloat()); //Placeholders
  ymovement(int(potvalue)); //Placeholders
  // Electromagnet(0); //Placeholders

}

void xzmovement(float xvalue, float yvalue) {
  /*
     Will recieve a value from 0 to 1023
     If 512 < value, move forward
     If 512 > value, move backward
  */

  X = map(xvalue, 0, 1023, -1, 1);
  Z = map(yvalue, 0, 1023, -1, 1);
  //Z = (yvalue / 1023.0) * 2 - 1;
  Left = X + Z;
  Right = X - Z;
  if (Left > 1) {
    Left = 1;
  } else if (Left < -1) {
    Left = -1;
  } else if (Right > 1) {
    Right = 1;
  } else if (Right < -1) {
    Right = -1;
  } 

  //DutyCycleL = Left * 1023.0;
  //DutyCycleR = Right * 1023.0;
  DutyCycleL = map(Left, -1, 1, -1023, 1023);
  DutyCycleR = map(Right, -1, 1, -1023, 1023);
  Serial.println(String(DutyCycleL) + "  " + String(DutyCycleR));
  if (DutyCycleL >= 0) {
    ledcWrite(pwmChannel3, DutyCycleL);
    ledcWrite(pwmChannel4, 0);
  } else if (DutyCycleL < 0) {
    ledcWrite(pwmChannel3, 0);
    ledcWrite(pwmChannel4, abs(DutyCycleL));
  }
  
  if (DutyCycleR >= 0) {
    ledcWrite(pwmChannel1, DutyCycleR);
    ledcWrite(pwmChannel2, 0);
  } else if (DutyCycleR < 0) {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, abs(DutyCycleR));
  }
  //delay(700);
}

void ymovement(int potvalue){
  DutyCycleU = ReceivedY;
  ledcWrite(pwmChannel5, DutyCycleU);  //
  ledcWrite(pwmChannel6, 0);           // Need to confirm which is up direction and which is down, placeholder values for now
  ledcWrite(pwmChannel7, DutyCycleU);  //
  ledcWrite(pwmChannel8, 0);           //
  }

/*  void Electromagnet(int buttvalue){
  //Need to check the direction and polarity of this
  if (ReceivedE%2 == 0){
    digitalWrite(magnet, HIGH);
  } else if (ReceivedE%2 != 0){
    digitalWrite(magnet, LOW);
  }
  }*/
