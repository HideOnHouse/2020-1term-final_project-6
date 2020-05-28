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
    move(3, maxHeight + 13);
    printw("%d", points);
    refresh();
}

bool SnakeClass::checkScore() {

}

void SnakeClass::initBoard() {
    move(2, maxHeight + 10);
    addstr("Score Board");
    move(3, maxHeight + 10);
    addstr("B:   (Current Length)/(Max Length)");//(Current Length)/(Max Length)
    move(4, maxHeight + 10);
    addstr("+:   (Obtained Growth Items)");
    move(5, maxHeight + 10);
    addstr("-:   (Obtained Poision Items)");
    move(6, maxHeight + 10);
    addstr("G:   (Gate Used)");
    move(9, maxHeight + 10);
    addstr("Mission");
    move(10, maxHeight + 10);
    addstr("B:");

    move(10, maxHeight + 13);
    printw("%d", points);


    move(11, maxHeight + 10);
    addstr("+:");
    move(12, maxHeight + 10);
    addstr("-:");

}

void SnakeClass::putGrowth(int whichGrowth = rand() % 2) {
    while (1) {
        int tmpx = rand() % maxWidth + 1;
        int tmpy = rand() % maxHeight + 1;
        for (int i = 0; i < snake.size(); ++i) {
            if (snake[i].x == tmpx && snake[i].y == tmpy) {
                continue;
            }
        }
        if (tmpx >= maxWidth - 2 || tmpy >= maxHeight - 3) {
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

void SnakeClass::putPoison(int whichPoison = rand() % 2) {
    while (1) {
        int tmpx = rand() % maxWidth + 1;
        int tmpy = rand() % maxHeight + 1;
        for (int i = 0; i < snake.size(); ++i) {
            if (snake[i].x == tmpx && snake[i].y == tmpy) {
                continue;
            }
        }
        if (tmpx >= maxWidth - 2 || tmpy >= maxHeight - 3) {
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
    // start put item
    if (growthCount + poisonCount < 3) {
        int fullItem = rand() % 2;
        if (fullItem) {
            while (growthCount + poisonCount == 3) {
                if (growthCount == 2) {
                    putPoison();
                } else if (poisonCount == 2) {
                    putGrowth();
                } else {
                    int temp = rand() % 2;
                    if (temp == 0) {
                        putGrowth();
                    } else {
                        putPoison();
                    }
                }
            }
        } else {
            if (growthCount == 2) {
                putPoison();
            } else if (poisonCount == 2) {
                putGrowth();
            } else {
                int temp = rand() % 2;
                if (temp == 0) {
                    putGrowth();
                } else {
                    putPoison();
                }
            }
        }
    }
    // end put item

    // start is snake collision or too short
    if (snake[0].x == 0 || snake[0].x == maxWidth - 2 || snake[0].y == 0 || snake[0].y == maxHeight - 2) {
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
    // end is snake collision or too short

    // get something?
    for (int j = 0; j < 2; ++j) {

        if (snake[0].x == growthItems[j].x && snake[0].y == growthItems[j].y) {
            getGrowth = true;
            int temp = rand() % 2;
            if (temp) {
                putGrowth(j);
            } else {
                putPoison();
            }
            points += 1;
            snakeLength += 1;
            displayScore();

            // get poison?
        } else if (snake[0].x == poisonItems[j].x && snake[0].y == poisonItems[j].y) {
            getPoison = true;
            int temp = rand() % 2;
            if (temp) {
                putGrowth(j);
            } else {
                putPoison();
            }
            points -= 1;
            snakeLength -= 1;
            displayScore();
        } else {
            getGrowth = false;
            getPoison = false;
        }
    }
    return false;
}

void SnakeClass::moveSnake() {
    int tmp = getch();
    switch (tmp) {
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
        refresh();
        snake.pop_back();
    }

    if (getPoison) {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        addch(' ');
        move(snake[snake.size() - 2].y, snake[snake.size() - 2].x);
        addch(' ');
        snake.pop_back();
        snake.pop_back();
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
    for (int i = 0; i < snake.size(); ++i) {
        move(snake[i].y, snake[i].x);
        if (i == 0) {
            addch(snakeHeadChar);
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
//    getmaxyx(stdscr, maxHeight, maxWidth);
    maxHeight = 21;
    maxWidth = 21;

    //init variables
    snakeLength = 3;
    growthCount = 0;
    poisonCount = 0;

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
        snake.push_back(snakePart(maxWidth / 2 + i, maxHeight / 2));
    }
    points = 0;
    tick = 200000;
    getGrowth = false;
    getPoison = false;
    direction = 'l';
    srand(time(0));
    putGrowth();

    //draw the edge
    for (int j = 0; j < maxWidth - 1; ++j) {
        move(maxHeight - 2, j);
        addch(wallChar);
    }
    for (int k = 0; k < maxHeight - 1; ++k) {
        move(k, maxWidth - 2);
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
    putGrowth();
    putPoison();
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
            move(maxWidth / 2 - 4, maxHeight / 2);
            printw("Game Over");
            break;
        }
        //displayScore();
        moveSnake();
        if (direction == 'q') {
            break;
        }

        // if(checkScore) {
        //     move(maxWidth / 2 - 4, maxHeight / 2);
        //     printw("Game Over");
        //     break;
        // }
        usleep(tick);
    }
}
