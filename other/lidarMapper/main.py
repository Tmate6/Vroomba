import re
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

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

'''def detectFullRotation(data, windowLen):
    threshold = 0
    while True:
        print(f'[----------------------------------]')
        print(f'[ Index | Data | Comp | +- | Match ]')
        print(f'[-------+------+------+----+-------]')
        
        startWindow = data[0:windowLen]
        processingData = data[windowLen:]
        print(startWindow)

        checkWindow = []
        matched = 0

        for i, distance in enumerate(processingData):
            print(f'[ {i+windowLen}{(5-len(str(i+windowLen))) * " "} | {distance}{(4-len(str(distance))) * " "} | {startWindow[matched]}{(4-len(str(startWindow[matched]))) * " "} | {bcolors.OKGREEN if threshold <= 5 else bcolors.WARNING if threshold <= 10 else bcolors.FAIL}{threshold}{(2-len(str(threshold))) * " "} |', end='')

            if distance <= (startWindow[matched] + threshold) and distance >= (startWindow[matched] - threshold):
                matched += 1
                checkWindow.append(distance)

                print(f' {bcolors.OKGREEN}{matched}{(5-len(str(matched))) * " "}{bcolors.ENDC} ]')
            else:
                matched = 0
                checkWindow = []

                print(f' {bcolors.FAIL}{matched}{(5-len(str(matched))) * " "}{bcolors.ENDC} ]')

            if matched == windowLen:
                print(f'[-----------------------------]')
                print(f'[     Match at index {i}{(6-len(str(i))) * " "}   ]')
                print(f'[-----------------------------]')
                return data[:i]
        threshold += 1
'''
def correctSensorOffset(data):
    newData = []
    for distance in data:
        newData.append(distance + 12)
        
    return newData

class Cell:
    def __init__(self) -> None:
        self.isWall = False
        self.isCenter = False

def mapPoints(data):
    data = correctSensorOffset(data)

    angles = np.linspace(0, 2*np.pi, len(data))
    x = data * np.cos(angles)
    y = data * np.sin(angles)

    cellSize = 15

    minX, maxX = min(x), max(x)
    minY, maxY = min(y), max(y)

    xSize = int((maxX - minX) / cellSize) + 1
    ySize = int((maxY - minY) / cellSize) + 1

    grid = [[Cell() for _ in range(ySize)] for _ in range(xSize)]

    for xCoord, yCoord in zip(x, y):
        gridX = int((xCoord - minX) / cellSize)
        gridY = int((yCoord - minY) / cellSize)
        grid[gridX][gridY].isWall = True

    gridX = int((0 - minX) / cellSize)
    gridY = int((0 - minY) / cellSize)
    grid[gridX][gridY].isCenter = True

    plotGrid(grid, cellSize, minX, maxX, minY, maxY)

def plotGrid(grid, cellSize, minX, maxX, minY, maxY):
    fig, ax = plt.subplots(figsize=(10, 10))
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            color = 'blue' if grid[i][j].isWall else 'red' if grid[i][j].isCenter else 'white'
            rect = patches.Rectangle((i*cellSize, j*cellSize), cellSize, cellSize, linewidth=1, edgecolor='black', facecolor=color)
            ax.add_patch(rect)

    plt.axis('equal')
    plt.show()

if __name__ == "__main__":
    mapPoints(data)