#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

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

const int cyclesInFullTurn = 500;

unsigned int scan[cyclesInFullTurn];

int startX;
int startY;
int startZ;

void setFullRotation() {
  sensors_event_t event;
  mag.getEvent(&event);
  
  startX = event.magnetic.x;
  startY = event.magnetic.y;
  startZ = event.magnetic.z;
}

bool checkFullRotation() {
  sensors_event_t event;
  mag.getEvent(&event);
/*
  Serial.print(startX); Serial.print(" "); Serial.println(event.magnetic.x);
  Serial.print(startY); Serial.print(" "); Serial.println(event.magnetic.y);
  */
  if (event.magnetic.x > (startX - 4) && event.magnetic.x < (startX + 4)) {
    if (event.magnetic.y > (startY - 4) && event.magnetic.y < (startY + 4)) {
      return true;
    }
  }
  return false;
}

void scanLidar() {
  setFullRotation();
  
  analogWrite(rightDisable, 180);
  analogWrite(leftDisable, 180);
  digitalWrite(leftBack, HIGH);
  digitalWrite(rightForward, HIGH);

  int turns = 0;

  while (true) {
    if (checkFullRotation() == true && turns > 100) {
      break;
    }
    
    Serial.println(readLidar());
    turns++;
  }

  digitalWrite(rightDisable, HIGH);
  digitalWrite(leftDisable, HIGH);
  digitalWrite(leftBack, LOW);
  digitalWrite(rightForward, LOW);
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

            return dist;
          }
        }
      }
    }
    delay(5);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  
  // Magnetometer
  if (!mag.begin()) {
    Serial.println("Magnetometer error");
    while(1);
  }
  
  Serial.println("Magnetometer working");

  // Motor
  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, HIGH);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, HIGH);

  pinMode(rightForward, OUTPUT); digitalWrite(rightForward, LOW);
  pinMode(rightBack, OUTPUT); digitalWrite(rightBack, LOW);
  pinMode(leftForward, OUTPUT); digitalWrite(leftForward, LOW);
  pinMode(leftBack, OUTPUT); digitalWrite(leftBack, LOW);

  // Lidar
  LidarSerial.begin(115200);
}

void loop() {
  Serial.println("[SCAN START]");
  delay(1200);
  scanLidar();
  Serial.println("[SCAN END]");
}
