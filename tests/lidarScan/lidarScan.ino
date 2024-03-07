// SD
#include "FS.h"
#include "SD.h"
#include "SPI.h"

const byte SdPin = 5;

// Magnetometer
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

int startX;
int startY;
int startZ;


// LiDAR
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include <SoftwareSerial.h>
SoftwareSerial LidarSerial(16, 17); // RX, TX

int dist;
int strength;
byte temp;

int check;
int lidarReadLoop;

int uart[9];
const int HEADER=0x59;

unsigned int scan[600];


// Motor
const byte rightDisable = 26;
const byte leftDisable = 25;

const byte rightForward = 27;
const byte rightBack = 14;
const byte leftForward = 32;
const byte leftBack = 33;

// Funcs
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

  Serial.print(event.magnetic.x); Serial.print(" "); Serial.print(startX); Serial.print(" "); Serial.print(event.magnetic.y); Serial.print(" "); Serial.println(startY);
  
  if (event.magnetic.x > (startX - 4) && event.magnetic.x < (startX + 4)) {
    if (event.magnetic.y > (startY - 4) && event.magnetic.y < (startY + 4)) {
      return true;
    }
  }
  return false;
}

void saveScan() {
  if (SD.remove("/scan.txt")) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
  
  File file = SD.open("/scan.txt", FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  for (int i = 0; i < 600; i++) {
    file.print(scan[i]);
    file.print(",");
  }
  
  file.close();
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
    
    scan[turns] = readLidar();
    turns++;
  }

  digitalWrite(rightDisable, HIGH);
  digitalWrite(leftDisable, HIGH);
  digitalWrite(leftBack, LOW);
  digitalWrite(rightForward, LOW);

  saveScan();
}

int readLidar() {
  while (true) {
    if (LidarSerial.available()) {
      if (LidarSerial.read() == HEADER) {
        uart[0]=HEADER;
        
        if (LidarSerial.read() == HEADER) {
          uart[1] = HEADER;
            
          for (lidarReadLoop = 2; lidarReadLoop < 9; lidarReadLoop++) {
            uart[lidarReadLoop] = LidarSerial.read();
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

  // Motor
  pinMode(rightDisable, OUTPUT); digitalWrite(rightDisable, HIGH);
  pinMode(leftDisable, OUTPUT); digitalWrite(leftDisable, HIGH);

  pinMode(rightForward, OUTPUT); digitalWrite(rightForward, LOW);
  pinMode(rightBack, OUTPUT); digitalWrite(rightBack, LOW);
  pinMode(leftForward, OUTPUT); digitalWrite(leftForward, LOW);
  pinMode(leftBack, OUTPUT); digitalWrite(leftBack, LOW);

  // Magnetometer
  if (!mag.begin()) {
    Serial.println("Magnetometer error");
    while (1);
  }
  
  Serial.println("Magnetometer initialised");

  // Lidar
  LidarSerial.begin(115200);

  // SD
  if (!SD.begin(SdPin)) {
    Serial.println("SD card module error");
    while (1);
  }

  if (SD.cardType() == CARD_NONE) {
    Serial.println("SD card not found");
    while (1);
  }

  Serial.println("SD card initialised");
}

void loop() {
  Serial.println("[SCAN START]");
  delay(1200);
  scanLidar();
  Serial.println("[SCAN END]");
}
