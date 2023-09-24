#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

byte M1d = 14;
byte M2d = 27;

byte M1dir1 = 26;
byte M1dir2 = 12;
byte M2dir1 = 33;
byte M2dir2 = 32;

const char *SSID = "ARRIS-25DF";
const char *PWD = "1D56D9B1FD31E39C";

WebServer server(80);

StaticJsonDocument<250> jsonDocument;
char buffer[250];

void goForward() {
  Serial.println("goForward");

  digitalWrite(M1d, LOW);
  digitalWrite(M2d, LOW);

  digitalWrite(M1dir1, LOW);
  digitalWrite(M1dir2, HIGH);
  digitalWrite(M2dir1, LOW);
  digitalWrite(M2dir2, HIGH);

  server.send(200, "application/json", buffer);
}

void goBack() {
  Serial.println("goBack");

  digitalWrite(M1d, LOW);
  digitalWrite(M2d, LOW);

  digitalWrite(M1dir1, HIGH);
  digitalWrite(M1dir2, LOW);
  digitalWrite(M2dir1, HIGH);
  digitalWrite(M2dir2, LOW);

  server.send(200, "application/json", buffer);
}

void goStop() {
  Serial.println("goStop");

  digitalWrite(M1d, HIGH);
  digitalWrite(M2d, HIGH);

  digitalWrite(M1dir1, LOW);
  digitalWrite(M1dir2, LOW);
  digitalWrite(M2dir1, LOW);
  digitalWrite(M2dir2, LOW);

  server.send(200, "application/json", buffer);
}

void routingSetup() {
  server.on("/forward", goForward);
  server.on("/back", goBack);
  server.on("/stop", goStop);
  
  server.begin();    
}
 

void handlePost() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  server.send(200, "application/json", "{}");
}

void setup() {     
  Serial.begin(115200); 

  Serial.print("Motor setup");

  pinMode(M1dir1, OUTPUT);
  pinMode(M1dir2, OUTPUT);
  pinMode(M2dir1, OUTPUT);
  pinMode(M2dir2, OUTPUT);
  pinMode(M1d, OUTPUT);
  pinMode(M2d, OUTPUT);

  digitalWrite(M1d, HIGH);
  digitalWrite(M2d, HIGH);

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("\nConnected. IP:");
  Serial.println(WiFi.localIP());
  routingSetup();     
   
}    
       
void loop() {    
  server.handleClient();   
}
