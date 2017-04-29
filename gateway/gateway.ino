#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include "setting.h"

#define SDA 4
#define SCL 5

#define DEVICEID 11

ESP8266WiFiMulti WiFiMulti;

bool isSent = false;

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

    if(x < 50 && !isSent) {
        post();
        isSent = true;
    } else {
         isSent = false;

      }
  }
  delay(100);
}

int readModule(byte addr) {
  Wire.requestFrom(addr, 1);
  if(Wire.available() == 1) {
    return Wire.read();
  }
  return -1;
}


void post() {
  Serial.println("Posting!!!!");
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String query = "http://133.16.123.101/api/sensor/";
    query += DEVICEID;
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


void post2() {
  Serial.println("Posting!!!!");
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String query = "http://133.16.123.101/api/sensor/";
    query += DEVICEID;
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

