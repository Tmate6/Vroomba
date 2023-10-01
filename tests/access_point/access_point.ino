#include "WiFi.h"
#include <WebServer.h>

#include <ArduinoJson.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* ssid = "ESP32-Soft-accessPoint";
const char* password = "123456789";

WebServer server(80);

StaticJsonDocument<250> jsonDocument;

void handlePost() {
  Serial.println("Request recieved");
  
  if (server.hasArg("plain") == false) {
    Serial.println("Request invalid - no body");
    server.send(400, "application/json", "{}");
    return;
  }
  
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  Serial.println(jsonDocument["right"].as<int>());
  
  server.send(200, "application/json", "{}");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  
  Serial.print("Setting soft access point mode");
  WiFi.softAP(ssid, password);
  
  
  server.on("/", HTTP_POST, handlePost);
  server.begin();
}

void loop() {
   server.handleClient();
}
