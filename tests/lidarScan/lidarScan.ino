#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include <SoftwareSerial.h>
SoftwareSerial LidarSerial(34, 19); // RX, TX

const byte rightDisable = 26;
const byte leftDisable = 25;

const byte rightForward = 27;
const byte rightBack = 14;
const byte leftForward = 32;
const byte leftBack = 33;

int dist;
int strength;
byte temp;

int check;
int i;

int uart[9];
const int HEADER=0x59;

unsigned int scan[500];

void scanLidar() {
  analogWrite(rightDisable, 170);
  analogWrite(leftDisable, 170);
  digitalWrite(leftForward, HIGH);
  digitalWrite(rightBack, HIGH);
  
  for (int turn = 0; turn < 500; turn++) {
    scan[turn] = readLidar();
    Serial.println(scan[turn]);
  }

  digitalWrite(rightDisable, HIGH);
  digitalWrite(leftDisable, HIGH);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightBack, LOW);
}

int readLidar() {
  while (true) {
    if (LidarSerial.available()) {
      if (LidarSerial.read() == HEADER) {
        uart[0]=HEADER;
        
        if (LidarSerial.read() == HEADER) {
          uart[1] = HEADER;
            
          for (i = 2; i < 9; i++) {
            uart[i] = LidarSerial.read();
          }
          
          check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
          
          if (uart[8] == (check & 0xff)) {
            if (uart[2] + uart[3]*256 < 0) {
            }
            
            dist = uart[2] + uart[3]*256;
            strength = uart[4] + uart[5]*256;
            temp = (uart[6] + uart[7]*256)/8 - 256;
            
            /*Serial.print("Distance: ");
            Serial.print(dist);
            Serial.print("cm, Strength: ");
            Serial.print(strength);
            Serial.print(", Chip Temp: ");
            Serial.print(temp);
            Serial.println("°C");*/

            return dist;
          }
        }
      }
    }
    delay(10);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, HIGH);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, HIGH);

  pinMode(rightForward, OUTPUT); digitalWrite(rightForward, LOW);
  pinMode(rightBack, OUTPUT); digitalWrite(rightBack, LOW);
  pinMode(leftForward, OUTPUT); digitalWrite(leftForward, LOW);
  pinMode(leftBack, OUTPUT); digitalWrite(leftBack, LOW);
  
  Serial.begin(115200);
  LidarSerial.begin(115200);
  
}

void loop() {
  delay(600);
  scanLidar();
}
