#include <SoftwareSerial.h>
SoftwareSerial LidarSerial(34, 19); // RX, TX

int dist;
int strength;
byte temp;

int check;
int i;

int uart[9];
const int HEADER=0x59;

unsigned int scan[500];


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
              return -1;
            }
            
            dist = uart[2] + uart[3]*256;
            strength = uart[4] + uart[5]*256;
            temp = (uart[6] + uart[7]*256)/8 - 256;
            
            Serial.print("Distance: ");
            Serial.print(dist);
            Serial.print("cm, Strength: ");
            Serial.print(strength);
            Serial.print(", Chip Temp: ");
            Serial.print(temp);
            Serial.println("°C");

            return dist;
          }
        }
      }
    }

    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  LidarSerial.begin(115200);
}

void loop() {
  readLidar();
}
