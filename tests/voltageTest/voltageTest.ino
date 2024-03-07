byte voltagePin = 13;

void setup() {
  Serial.begin(115200);
  pinMode(voltagePin, INPUT);
}

void loop() {
  double voltage = analogRead(voltagePin) * 0.0008058608 * 10.251256281;
  Serial.println(voltage);
  delay(100);
}
