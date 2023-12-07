#include "IRremote.h"

byte IRpin = 13;
byte rightDisable = 26;
byte leftDisable = 25;

IRrecv irrecv(IRpin);

decode_results results;

void setup() {
  Serial.begin(115200);
  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, HIGH);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, HIGH);
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
