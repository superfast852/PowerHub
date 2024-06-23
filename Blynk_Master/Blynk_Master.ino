#define BLYNK_PRINT Serial
#define ESPALEXA_DEBUG
#define ESPALEXA_ASYNC
#define ESPALEXA_MAXDEVICES 3
#include <WiFi.h>
#include <Wire.h>
#include <Filters.h>
#include <Espalexa.h>
#include <WiFiClient.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <BlynkSimpleEsp32.h>
#include "SPIFFS.h"
#include "FS.h"
float ACS_Value;
float testFrequency = 50;
float windowLength = 40.0/testFrequency;
float intercept = 0;
float slope = 0.0752;
float Amps_TRMS; // estimated actual current in amps
unsigned long printPeriod = 1000; // in milliseconds
unsigned long previousMillis = 0;
char ssid[32] = {};
char pass[32] = {};



char auth[] = "LP4H8TXoNiwEoJMq6NYnwD3lXdf67WNM";
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness2);
void thirdLightChanged(uint8_t brightness3);

BlynkTimer timer;
Espalexa espalexa;
EspalexaDevice* device;
EspalexaDevice* device2;
EspalexaDevice* device3;
AsyncWebServer server(80);
RunningStatistics inputStats;

void fetchSSID(){
  SPIFFS.begin();
  File file = SPIFFS.open("/ssid.txt");
  uint16_t i = 0;
  while(file.available()){
     ssid[i] = file.read();
     // Serial.print (VALUE [i]); //use for debug
     i++;
  }
  Serial.println(ssid); //use for debug
  file.close();
}

void fetchPWD(){
  SPIFFS.begin();
  File file = SPIFFS.open("/pwd.txt");
  uint16_t i = 0;
  while(file.available()){
     pass[i] = file.read();
     // Serial.print (VALUE [i]); //use for debug
     i++;
  }
  Serial.println(pass); //use for debug
  file.close();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Starting...");
  fetchSSID();
  fetchPWD();
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(33, INPUT);
  WiFi.begin(ssid,pass);
  Wire.beginTransmission(4);
  Wire.write(WiFi.localIP());
  Wire.endTransmission();
  device = new EspalexaDevice("Socket 1", firstLightChanged);
  device2 = new EspalexaDevice("Socket 2", secondLightChanged);
  device3 = new EspalexaDevice("Socket 3", thirdLightChanged);
  espalexa.addDevice(device);
  espalexa.addDevice(device2);
  espalexa.addDevice(device3);
  espalexa.begin(&server);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sync);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32. To see the current output values, go to /espalexa and to update the firmware, go to /update");
  });
  AsyncElegantOTA.begin(&server);
  espalexa.begin(&server);
  inputStats.setWindowSecs( windowLength );     //Set the window length  
}

BLYNK_WRITE(V1){
  int val = param.asInt();
  if(val){
    digitalWrite(13, HIGH);
    device->setValue(255);
  } else {
    digitalWrite(13, LOW);
    device->setValue(0);
  }
}

BLYNK_WRITE(V2){
  int val = param.asInt();
  if(val){
    digitalWrite(12, HIGH);
    device2->setValue(255);
  } else {
    digitalWrite(12, LOW);
    device2->setValue(0);
  }
}

BLYNK_WRITE(V3){
  int val = param.asInt();
  if(val){
    digitalWrite(14, HIGH);
    device3->setValue(255);
  } else {
    digitalWrite(14, LOW);
    device3->setValue(0);
  }
}

void loop(){
    Blynk.run();
    timer.run();
    espalexa.loop();
    delay(1);
}

void firstLightChanged(uint8_t brightness){
  if(brightness<=1){
    digitalWrite(13, HIGH);
    Blynk.virtualWrite(V1, HIGH);
  }else{
    digitalWrite(13, LOW);
    Blynk.virtualWrite(V1, LOW);
  }
}
void secondLightChanged(uint8_t brightness2){
  if(brightness2<=1){
    digitalWrite(12, HIGH);
    Blynk.virtualWrite(V2, HIGH);
  }else{
    digitalWrite(12, LOW);
    Blynk.virtualWrite(V2, LOW);
  }
}
void thirdLightChanged(uint8_t brightness3){
  if(brightness3<=1){
    digitalWrite(14, HIGH);
    Blynk.virtualWrite(V3, HIGH);
  }else{
    digitalWrite(14, LOW);
    Blynk.virtualWrite(V3, LOW);
  }
}

void sync(){
  ACS_Value = analogRead(33);  // read the analog in value:
  inputStats.input(ACS_Value);  // log to Stats functionszx
  int Amps_TRMS = intercept + slope * inputStats.sigma();
  Wire.beginTransmission(4);
  Wire.write(Amps_TRMS);
  Wire.endTransmission();
  Blynk.virtualWrite(V5, Amps_TRMS);
  Serial.println(Amps_TRMS);
}
