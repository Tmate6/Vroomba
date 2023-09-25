#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

WebServer server(80);
 
StaticJsonDocument<250> jsonDocument;
char buffer[250];

byte rightDisable = 26;
byte leftDisable = 25;

byte rightForward = 27;
byte rightBack = 14;
byte leftForward = 32;
byte leftBack = 33;

void controlMovement(int right, int left) {
  Serial.print("Request details - right: "); Serial.print(right); Serial.print(", left: "); Serial.println(left);

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
}

void handlePost() {
  Serial.println("Request recieved");
  
  if (server.hasArg("plain") == false) {
    Serial.println("Request invalid - no body");
    server.send(400, "application/json", "{}");
    return;
  }
  
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  controlMovement(jsonDocument["right"], jsonDocument["left"]);
  
  server.send(200, "application/json", "{}");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);

  Serial.println("\nMotor setup");
  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, HIGH);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, HIGH);
  
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBack, OUTPUT);

  
  Serial.print("\nConnecting to wifi");
  WiFi.begin("ARRIS-25DF", "1D56D9B1FD31E39C");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_POST, handlePost);
  server.begin();
  Serial.print("Server running, accepting requests.");
}    
       
void loop() {    
  server.handleClient();     
}
