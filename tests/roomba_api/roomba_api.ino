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

String outputcm;

int triggerpin = 19;
int echopin = 34;

float distance;

bool goingForward;

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

  if (right > 0 || left > 0) {
    goingForward = true;
  } else {
    goingForward = false;
  }
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

  Serial.println("Proximity setup");
  pinMode(triggerpin, OUTPUT);
  pinMode(echopin, INPUT);
  
  Serial.print("Connecting to wifi");
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
  digitalWrite(triggerpin, LOW);
  delayMicroseconds(10);
  digitalWrite(triggerpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin, LOW);
  
  distance = (pulseIn(echopin, HIGH)*765.*5280.*12)/(3600.*1000000.)/2*2.54;
  
  if (distance < 10.0 && goingForward == true) {
    Serial.println("distance");
    controlMovement(0, 0);
  } 
  
  server.handleClient();
}
