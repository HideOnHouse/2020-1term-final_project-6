//
// Created by HideOnHouse on 2020-05-16.
//
/*
 * TODO List
 * - Distinct snakeHead and snakeBody (solved)
 * - Implement gate
 * - Implement scoreBoard
 * - Add Obstacle
 * - Put item randomly 정(max item 3)정
 * - popup Press any button to start when start game
 * - add stage
 */

#include <time.h>
#include <zconf.h>
#include "snake.h"
#include "ncurses.h"

snakePart::snakePart(int col, int row) {
    x = col;
    y = row;
}

snakePart::snakePart() {
    x = 0;
    y = 0;
}


void SnakeClass::displayScore() {

    //write the points
    move(3, screenHeight + 13);
    printw("%d", snakeLength);
    move(4, screenHeight + 13);
    printw("%d", totalGrowth);
    move(5, screenHeight + 13);
    printw("%d", totalPoison);
    refresh();
}

bool SnakeClass::checkScore() {

}

void SnakeClass::initBoard() {
    move(2, screenHeight + 10);
    addstr("Score Board");
    move(3, screenHeight + 10);
    addstr("B: 3 (Current Length)/(Max Length)");//(Current Length)/(Max Length)
    move(4, screenHeight + 10);
    addstr("+: 0 (Obtained Growth Items)");
    move(5, screenHeight + 10);
    addstr("-: 0 (Obtained Poison Items)");
    move(6, screenHeight + 10);
    addstr("G: 0 (Gate Used)");
    move(9, screenHeight + 10);
    addstr("Mission");
    move(10, screenHeight + 10);
    addstr("B:");

    move(10, screenHeight + 13);
    printw("%d", points);


    move(11, screenHeight + 10);
    addstr("+:");
    move(12, screenHeight + 10);
    addstr("-:");

}

void SnakeClass::putGrowth(int whichGrowth) {
    while (1) {
        int tmpx = rand() % screenWidth + 1;
        int tmpy = rand() % screenHeight + 1;
        for (int i = 0; i < snake.size(); ++i) {
            if (snake[i].x == tmpx && snake[i].y == tmpy) {
                continue;
            }
        }
        for (int j = 0; j < 2; ++j) {
            if (tmpx == growthItems[j].x && tmpy == growthItems[j].y) {
                continue;
            }
            if (tmpx == poisonItems[j].x && tmpy == poisonItems[j].y) {
                continue;
            }
        }
        if (tmpx >= screenWidth - 2 || tmpy >= screenHeight - 3) {
            continue;
        }

        if (growthItems[whichGrowth].x == tmpx && growthItems[whichGrowth].y == tmpy) {
            continue;
        }
        growthItems[whichGrowth].x = tmpx;
        growthItems[whichGrowth].y = tmpy;
        break;
    }
    move(growthItems[whichGrowth].y, growthItems[whichGrowth].x);
    addch(growthItemChar);
    growthCount += 1;
    refresh();
}

void SnakeClass::putPoison(int whichPoison) {
    while (1) {
        int tmpx = rand() % screenWidth + 1;
        int tmpy = rand() % screenHeight + 1;
        for (int i = 0; i < snake.size(); ++i) {
            if (snake[i].x == tmpx && snake[i].y == tmpy) {
                continue;
            }
        }
        for (int j = 0; j < 2; ++j) {
            if (tmpx == growthItems[j].x && tmpy == growthItems[j].y) {
                continue;
            }
            if (tmpx == poisonItems[j].x && tmpy == poisonItems[j].y) {
                continue;
            }
        }
        if (tmpx >= screenWidth - 2 || tmpy >= screenHeight - 3) {
            continue;
        }
        if (poisonItems[whichPoison].x == tmpx && poisonItems[whichPoison].y == tmpy) {
            continue;
        }
        poisonItems[whichPoison].x = tmpx;
        poisonItems[whichPoison].y = tmpy;
        break;
    }
    move(poisonItems[whichPoison].y, poisonItems[whichPoison].x);
    addch(poisonItemChar);
    poisonCount += 1;
    refresh();
}

bool SnakeClass::collision() {

    // check if snake is too short is collision with wall
    if (snake[0].x == 0 || snake[0].x == screenWidth - 2 || snake[0].y == 0 || snake[0].y == screenHeight - 2) {
        return true;
    }
    for (int i = 2; i < snake.size(); ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true;
        }
    }
    if (snakeLength < 3) {
        return true;
    }


    // get something?
    for (int j = 0; j < 2; ++j) {
        // get growth?
        if (snake[0].x == growthItems[j].x && snake[0].y == growthItems[j].y) {
            getGrowth = true;
            if (growthCount == 2) {
                putPoison(j);
                growthCount -= 2;
                poisonCount -= 1;
            } else {
                putGrowth(j);
                growthCount -= 1;
                poisonCount -= 2;
            }
            points += 1;
            totalGrowth += 1;
            snakeLength += 1;
            displayScore();
            break;

            // get poison?
        } else if (snake[0].x == poisonItems[j].x && snake[0].y == poisonItems[j].y) {
            getPoison = true;
            if (poisonCount == 2) {
                putGrowth(j);
                growthCount -= 1;
                poisonCount -= 2;
            } else {
                putPoison(j);
                growthCount -= 2;
                poisonCount -= 1;
            }
            totalPoison += 1;
            points -= 1;
            snakeLength -= 1;
            displayScore();
            break;

        } else {
            getGrowth = false;
            getPoison = false;
        }
    }
    return false;
}

void SnakeClass::moveSnake() {
    int currentKey = getch();
    switch (currentKey) {
        case KEY_LEFT:
            if (direction != 'r') {
                direction = 'l';
            }
            break;
        case KEY_UP:
            if (direction != 'd') {
                direction = 'u';
            }
            break;
        case KEY_RIGHT:
            if (direction != 'l') {
                direction = 'r';
            }
            break;
        case KEY_DOWN:
            if (direction != 'u') {
                direction = 'd';
            }
            break;
        case KEY_BACKSPACE:
            direction = 'q';
            break;
    }

    if (!getGrowth) {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        addch(' ');
        snake.pop_back();
        refresh();
    }

    if (getPoison) {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        addch(' ');
        snake.pop_back();
        refresh();
    }

    if (direction == 'l') {
        snake.insert(snake.begin(), snakePart(snake[0].x - 1, snake[0].y));
    } else if (direction == 'r') {
        snake.insert(snake.begin(), snakePart(snake[0].x + 1, snake[0].y));
    } else if (direction == 'u') {
        snake.insert(snake.begin(), snakePart(snake[0].x, snake[0].y - 1));
    } else if (direction == 'd') {
        snake.insert(snake.begin(), snakePart(snake[0].x, snake[0].y + 1));
    }

    //draw snake, check poison
    for (int i = 0; i < snake.size(); ++i) {
        move(snake[i].y, snake[i].x);
        if (i == 0) {
            addch(snakeHeadChar);
        } else if (i == snake.size() && getPoison) {
            addch(' ');
            snake.pop_back();
        } else {
            addch(snakeBodyChar);
        }
    }
    refresh();
}

SnakeClass::SnakeClass() {
    initscr();
    nodelay(stdscr, true); // the program not wait until the user press a key
    keypad(stdscr, true);
    noecho();
    curs_set(0);
//    getmaxyx(stdscr, screenHeight, screenWidth);
    screenHeight = 30;
    screenWidth = 30;

    //init variables
    snakeLength = 3;
    growthCount = 0;
    poisonCount = 0;
    totalGrowth = 0;
    totalPoison = 0;

    // start init item location
    for (int m = 0; m < 2; ++m) {
        growthItems[m].x = 0;
        growthItems[m].y = 0;
        poisonItems[m].x = 0;
        poisonItems[m].y = 0;
    }
    // end init item location

    snakeHeadChar = '3';
    snakeBodyChar = '4';
    wallChar = '1';
    immuneWallChar = '2';
    growthItemChar = '*';
    poisonItemChar = 'x';
    strcpy(scoreBoardChar, "Score Board");

    for (int i = 0; i < 3; ++i) {
        snake.push_back(snakePart(screenWidth / 2 + i, screenHeight / 2));
    }
    points = 0;
    tick = 200000;
    getGrowth = false;
    getPoison = false;
    direction = 'l';
    srand(time(0));

    //draw the edge
    for (int j = 0; j < screenWidth - 1; ++j) {
        move(screenHeight - 2, j);
        addch(wallChar);
    }
    for (int k = 0; k < screenHeight - 1; ++k) {
        move(k, screenWidth - 2);
        addch(wallChar);
    }

    //Initial - draw the snake
    for (int l = 0; l < snake.size(); ++l) {
        move(snake[l].y, snake[l].x);
        if (l == 0) {
            addch(snakeHeadChar);
        } else {
            addch(snakeBodyChar);
        }
    }

    //draw the items
    putGrowth(0);
    putGrowth(1);
    putPoison(0);
    initBoard();
    refresh();
    //displayScore();
}

SnakeClass::~SnakeClass() {
    nodelay(stdscr, false);
    getch();
    endwin();
}

void SnakeClass::start() {
    while (1) {
        if (collision()) {
            move(screenWidth / 2 - 4, screenHeight / 2);
            printw("Game Over");
            break;
        }
        //displayScore();
        moveSnake();
        if (direction == 'q') {
            break;
        }

        // if(checkScore) {
        //     move(screenWidth / 2 - 4, screenHeight / 2);
        //     printw("Game Over");
        //     break;
        // }
        usleep(tick);
    }
}
