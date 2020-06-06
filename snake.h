//
// Created by FURO on 2020-05-16.
//

#ifndef SNAKEPRACTICE_SNAKE_H
#define SNAKEPRACTICE_SNAKE_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>

struct snakePart {
    int x, y;

    snakePart(int col, int row);

    snakePart();
};

struct gatePart {
    int x, y;
    int doorX, doorY;
};

class SnakeClass {
    int snakeLength, cntGate, growthCount, poisonCount, totalGrowth, totalPoison;
    int points, tick, stageWidth, stageHeight;
    char direction, snakeHeadChar, snakeBodyChar, wallChar, immuneWallChar, growthItemChar, poisonItemChar, gateChar;
    char scoreBoardChar[100];
    bool getGrowth, getPoison, gameOver, gameClear;
    int snakeMaxLength;
    int endScore, missionGrowth, missionPoison, missionGate;
    int meetGate;
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

    bool collision();

    void refreshSnake();

public:
    SnakeClass();

    ~SnakeClass();

    void start();
};


#endif //SNAKEPRACTICE_SNAKE_H