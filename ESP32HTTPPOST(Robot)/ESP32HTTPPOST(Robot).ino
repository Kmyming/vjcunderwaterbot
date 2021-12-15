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

void setup() {
  //Serial Initialisation
  Serial.begin(115200);

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
}

void loop() {
  unsigned long currentMillis = millis();
    //check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      getRequest = httpGETRequest(serverNamepot);
      xvalue = getValue(getRequest, ' ', 3);
      yvalue = getValue(getRequest, ' ', 2);
      buttvalue = getValue(getRequest, ' ', 1);
      potvalue = getValue(getRequest, ' ', 0);
      xvalue.toInt();
      yvalue.toInt();
      buttvalue.toInt();
      potvalue.toInt();
    Serial.print ("x ");
    Serial.print(xvalue);
    Serial.print(" y "); 
    Serial.print(yvalue);
    Serial.println();
    Serial.println(buttvalue);
    Serial.println(potvalue);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
