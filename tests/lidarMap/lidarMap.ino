#include <math.h>

struct Cell {
    bool isWall = false;
};

void setup() {
  Serial.begin(115200);
}

void handleData(String dataString) {
  int arrayLen = 0;
  for (int i = 0; i < dataString.length(); i++) {
    if (dataString[i] == ',') {
      arrayLen++;
    }
  }

  Cell** grid;
  int xSize, ySize;

  int* data = new int[arrayLen];
  memset(data, 0, arrayLen * sizeof(int));

  int commas = 0;
  for (int i = 0; i < dataString.length(); i++) {
    if (dataString[i] == ',') {
      commas++;
      continue;
    }
    data[commas] = String(String(data[commas]) + String(dataString[i])).toInt();
  }

  float angles[arrayLen];
  float x[arrayLen];
  float y[arrayLen];

  int cellSize = 32;

  for (int i = 0; i < arrayLen; i++) {
    angles[i] = float(i) / float(arrayLen) * 2.0 * PI;
    x[i] = data[i] * cos(angles[i]);
    y[i] = data[i] * sin(angles[i]);
  }

  float minX = x[0], maxX = x[0];
  float minY = y[0], maxY = y[0];
  
  for (int i = 1; i < arrayLen; i++) {
    if (x[i] < minX) minX = x[i];
    if (x[i] > maxX) maxX = x[i];
    if (y[i] < minY) minY = y[i];
    if (y[i] > maxY) maxY = y[i];
  }

  xSize = int((maxX - minX) / cellSize) + 1;
  ySize = int((maxY - minY) / cellSize) + 1;

  grid = new Cell*[xSize];
  for (int i = 0; i < xSize; i++) {
    grid[i] = new Cell[ySize];
  }

  for (int i = 0; i < arrayLen; i++) {
    int gridX = int((x[i] - minX) / cellSize);
    int gridY = int((y[i] - minY) / cellSize);
    grid[gridX][gridY].isWall = true;
  }

  for (int i = 0; i < xSize; i++) {
    for (int j = 0; j < ySize; j++) {
      Serial.print(grid[i][j].isWall);
      Serial.print(" ");
    }
    Serial.println();
  }

  delete[] data;
}


void loop() {
  while (!Serial.available());
  handleData(Serial.readString());
  Serial.println();
}
