String outputcm;

int triggerpin = 19;
int echopin = 34;

int pingtime;
float pingdistance;

float fakedistance;
float distance;

void setup() {
  pinMode(triggerpin, OUTPUT);
  pinMode(echopin, INPUT);
  
  Serial.begin(115200);
}

void loop() {
  digitalWrite(triggerpin, LOW);
  delayMicroseconds(10);
  digitalWrite(triggerpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin, LOW);
  
  pingtime = pulseIn(echopin, HIGH);
  pingdistance = (pingtime*765.*5280.*12)/(3600.*1000000.);
  fakedistance = pingdistance/2;
  distance = fakedistance*2.54;
  
  if (distance < 0) outputcm = "ERROR";
  else outputcm = distance;
  
  delay(100);
  
  Serial.println(outputcm);
}
