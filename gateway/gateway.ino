#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include "setting.h"

#define SDA 4
#define SCL 5

#define TH 65

ESP8266WiFiMulti WiFiMulti;

bool isSent = false;
bool isPressed = false;

typedef enum State {
  normal,
  active,
  fall  
};

State state;

void setup() {
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  for(uint8_t t = 2; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFiMulti.addAP(SSID, PASSWORD);  
}

void loop() {
  for(int i=1; i<128; i++) {
    int x = readModule(i);
    if(x == -1) {
      continue;
    }
    Serial.print("Sensor");
    Serial.print(i);
    Serial.print(":= ");
    Serial.println(x);
    
    if(0 < x && x < TH && !isPressed) {
      Serial.print("Sensor");
      Serial.print(i);
      Serial.print(":= ");
      Serial.println("Active");
      isPressed = true;
      state = active;
      isSent = false;
      if(isLEDModuleConnected(13)) {
        Wire.beginTransmission(13); // transmit to device #8
        Wire.write(1);              // sends one byte
        Wire.endTransmission();    // stop transmitting
      }
    } else if(x > TH && isPressed) {
      Serial.print("Sensor");
      Serial.print(i);
      Serial.print(":= ");
      Serial.println("Normal");
      isPressed = false;
      state = normal;
      isSent = false;
      if(isLEDModuleConnected(13)) {
        Wire.beginTransmission(13); // transmit to device #8
        Wire.write(0);              // sends one byte
        Wire.endTransmission();    // stop transmitting
      }
    }

    if(!isSent) {
      if(isPressed) {
        activate(i);
      } else {
        normalize(i);
      }
      isSent = true;
    }
  }
  delay(100);
}

bool isLEDModuleConnected(int addr) {
  Wire.requestFrom(addr, 1);
  if(Wire.available() > 0) {
    Wire.read();
    return true;
  }
  return false;
}

int readModule(byte addr) {
  Wire.requestFrom(addr, 1);
  if(Wire.available() > 0) {
    return Wire.read();
  }
  return -1;
}

void activate(int deviceId) {
  Serial.println("Posting!!!!");
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String query = "http://133.16.123.101/api/sensor/";
    query += deviceId;
    query += "/Active";
    http.begin(query); //HTTP
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}


void normalize(int deviceId) {
  Serial.println("Normal!!!!");
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String query = "http://133.16.123.101/api/sensor/";
    query += deviceId;
    query += "/Normal";
    http.begin(query); //HTTP
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

