byte M1d = 26;
byte M2d = 25;

byte M1dir1 = 14;
byte M1dir2 = 27;
byte M2dir1 = 33;
byte M2dir2 = 32;

int i = 0;
 
void setup() {
  pinMode(M1dir1, OUTPUT);
  pinMode(M1dir2, OUTPUT);
  pinMode(M2dir1, OUTPUT);
  pinMode(M2dir2, OUTPUT);
  
  pinMode(M1d, OUTPUT);
  pinMode(M2d, OUTPUT);
  Serial.begin(115200);

  digitalWrite(M1dir1, LOW);
  digitalWrite(M1dir2, HIGH);
  digitalWrite(M2dir1, LOW);
  digitalWrite(M2dir2, HIGH);
 
}
 
void loop() {
  i++;
  if (i == 200) {i = 0;}
  analogWrite(M1d, 200-i);
  analogWrite(M2d, 200-i);
  delay(50);
}
