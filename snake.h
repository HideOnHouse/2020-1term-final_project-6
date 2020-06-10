//
// Created by FURO on 2020-05-16.
//

#ifndef SNAKEPRACTICE_SNAKE_H
#define SNAKEPRACTICE_SNAKE_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "stageClass.h"

struct snakePart {
    int x, y, time;

    snakePart(int col, int row);

    snakePart();
};

struct gatePart {
    int x, y;
    int doorX, doorY;
};

class SnakeClass {
    int snakeLength, cntGate, growthCount, poisonCount, totalGrowth, totalPoison;
    int points, tick, stageWidth, stageHeight, itemTick;
    char direction, snakeHeadChar, snakeBodyChar, wallChar, immuneWallChar, growthItemChar, poisonItemChar, gateChar, currentChar;
    char scoreBoardChar[100];
    bool getGrowth, getPoison, gameOver, gameClear;
    int endScore, missionGrowth, missionPoison, missionGate;
    int meetGate;
    int nextStage;
    gatePart gatePair[2];
    snakePart growthItems[2];
    snakePart poisonItems[2];
    std::vector<snakePart> snake;

    void initBoard() const;

    void displayScore() const;

    bool checkScore() const;

    void putGrowth(int whichGrowth);

    void putPoison(int whichPoison);

    void putGate();

    void findWayOut(int whichGate);

    void removeGate();

    void checkGate();

    bool collision();

    void refreshSnake();

public:
    SnakeClass(int whichStage);

    ~SnakeClass();

    bool start();
};


#endif //SNAKEPRACTICE_SNAKE_H