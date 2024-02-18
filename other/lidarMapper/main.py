import re
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
from collections import deque

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

with open("data.yml", 'r') as file:
    stringData = file.read().split(",")
    data = []
    for dataPoint in stringData:
        if dataPoint == "0":
            continue
        try:
            data.append(int(dataPoint))
        except:
            pass

print(data)

def correctSensorOffset(data):
    newData = []
    for distance in data:
        newData.append(distance + 17)
        
    return newData

class Cell:
    def __init__(self) -> None:
        self.innerPixels = 0
        self.isAvaliable = False
        self.isWall = False
        self.isCenter = False

def bresenham(x0, y0, x1, y1):
    points = []
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1
    if dx > dy:
        err = dx / 2.0
        while x != x1:
            points.append((x, y))
            err -= dy
            if err < 0:
                y += sy
                err += dx
            x += sx
    else:
        err = dy / 2.0
        while y != y1:
            points.append((x, y))
            err -= dx
            if err < 0:
                x += sx
                err += dy
            y += sy      
    points.append((x, y))
    return points

def hasLineOfSight(grid, start, end):
    points = list(bresenham(start[0], start[1], end[0], end[1]))
    return all(not grid[x][y].isWall for x, y in points)

def mapPoints(data):
    data = correctSensorOffset(data)

    angles = np.linspace(0, 2*np.pi, len(data))
    x = data * np.cos(angles)
    y = data * np.sin(angles)

    cellSize = 32

    minX, maxX = min(x), max(x)
    minY, maxY = min(y), max(y)

    xSize = int((maxX - minX) / cellSize) + 1
    ySize = int((maxY - minY) / cellSize) + 1

    grid = [[Cell() for _ in range(ySize)] for _ in range(xSize)]

    for xCoord, yCoord in zip(x, y):
        gridX = int((xCoord - minX) / cellSize)
        gridY = int((yCoord - minY) / cellSize)
        grid[gridX][gridY].isWall = True

    # Line-of-sight from start point
    startX, startY = int((0 - minX) / cellSize), int((0 - minY) / cellSize)
    for x in range(xSize):
        for y in range(ySize):
            if not grid[x][y].isWall and hasLineOfSight(grid, (startX, startY), (x, y)):
                grid[x][y].isAvaliable = True

    plotGrid(grid, cellSize, minX, maxX, minY, maxY)

def plotGrid(grid, cellSize, minX, maxX, minY, maxY):
    fig, ax = plt.subplots(figsize=(10, 10))
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j].isWall:
                color = 'blue'
            elif grid[i][j].isAvaliable:
                color = 'green'
            else:
                color = 'white'

            if grid[i][j].isCenter:
                color = 'red'
            rect = patches.Rectangle((i*cellSize, j*cellSize), cellSize, cellSize, linewidth=1, edgecolor='black', facecolor=color)
            ax.add_patch(rect)

    plt.axis('equal')
    plt.show()

if __name__ == "__main__":
    mapPoints(data)