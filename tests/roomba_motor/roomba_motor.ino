int speedPin = 27;
int M1dir1 = 26;
int M1dir2 = 25;
int M2dir1 = 33;
int M2dir2 = 32;
 
void setup() {
  // put your setup code here, to run once:
pinMode(M1dir1, OUTPUT);
pinMode(M1dir2, OUTPUT);
pinMode(M2dir1, OUTPUT);
pinMode(M2dir2, OUTPUT);
Serial.begin(115200);


digitalWrite(M1dir1, LOW);
  digitalWrite(M1dir2, HIGH);
digitalWrite(M2dir1, LOW);
  digitalWrite(M2dir2, HIGH);
 
}
 
void loop() {
}
