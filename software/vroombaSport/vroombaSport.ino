#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* ssid = "Vroomba";
const char* password = "123456789";

WebServer server(80);
 
StaticJsonDocument<250> jsonDocument;

byte rightDisable = 26;
byte leftDisable = 25;

byte rightForward = 27;
byte rightBack = 14;
byte leftForward = 32;
byte leftBack = 33;

bool goingForward;

void controlMovement(int right, int left) {
  //Serial.print("Request details - right: "); Serial.print(right); Serial.print(", left: "); Serial.println(left);

  if (right > 0) {
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightBack, LOW);
  } else {
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBack, HIGH);
  }

  if (left > 0) {
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftBack, LOW);
  } else {
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBack, HIGH);
  }
  
  analogWrite(rightDisable, 255-abs(right));
  analogWrite(leftDisable, 255-abs(left));

  if (right > 0 || left > 0) {
    goingForward = true;
  } else {
    goingForward = false;
  }
}

void handlePost() {
  //Serial.println("Request recieved");
  
  String body = server.arg("plain");
  Serial.println(body);
  deserializeJson(jsonDocument, body);
  
  controlMovement(int(jsonDocument["right"]) * 2.55, int(jsonDocument["left"]) * 2.55);
  
  server.send(200, "application/json", "{}");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, LOW);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, LOW);
  
  //Serial.begin(115200);

  //Serial.println("Motor setup");
  
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBack, OUTPUT);

  //Serial.println("Starting access point");
  WiFi.softAP(ssid, password);
  
  server.on("/", HTTP_POST, handlePost);
  server.begin();
  
  //Serial.print("Server running, accepting requests.");
}

void loop() {
  server.handleClient();
}
