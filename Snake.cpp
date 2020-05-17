//
// Created by HideOnHouse on 2020-05-16.
//
/*
 * TODO List
 * - Distinct snakeHead and snakeBody
 * - Implement gate
 * - Implement scoreBoard
 * - Add Obstacle
 * - Put item randomly (max item 3)
 * -
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

void SnakeClass::putGrowth() {
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

        if (poisonItem.x == tmpx && poisonItem.y == tmpy) {
            continue;
        }
        growthItem.x = tmpx;
        growthItem.y = tmpy;
        break;
    }
    move(growthItem.y, growthItem.x);
    addch(growthItemChar);
    refresh();
}

void SnakeClass::putPoison() {
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
        if (growthItem.x == tmpx && growthItem.y == tmpy) {
            continue;
        }
        poisonItem.x = tmpx;
        poisonItem.y = tmpy;
        break;
    }
    move(poisonItem.y, poisonItem.x);
    addch(poisonItemChar);
    refresh();
}

bool SnakeClass::collision() {
    if (snake[0].x == 0 || snake[0].x == maxWidth - 1 || snake[0].y == 0 || snake[0].y == maxHeight - 2) {
        return true;
    }
    for (int i = 2; i < snake.size(); ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true;
        }
    }
    if (snake[0].x == growthItem.x && snake[0].y == growthItem.y) {
        getGrowth = true;
        putGrowth();
        points += 1;
        //Draw Score
        move(maxHeight - 1, 0);
        printw("%d", points);
    } else if (snake[0], x == poisonItem.x && snake[0].y == poisonItem.y) {
        getPoison = true;
        putPoison();
        points -= 1;
        //Draw Score
        move(maxHeight - 1, 0);
        printw("%d", points)
    } else {
        getGrowth = false;
        getPoison = false;
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
    move(snake[0].y, snake[0].x);
    addch(snakeBodyChar);
    refresh();
}

SnakeClass::SnakeClass() {
    initscr();
    nodelay(stdscr, true); // the program not wait until the user press a key
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, maxHeight, maxWidth);
    //init a few variables
    snakeHeadChar = '3';
    snakeBodyChar = '4';
    wallChar = '1';
    immuneWallChar = '2';
    growthItemChar = '*';
    growthItem.x = 0;
    growthItem.y = 0;
    poisonItem.x = 0;
    poisonItem.y = 0;
    for (int i = 0; i < 3; ++i) {
        snake.push_back(snakePart(40 + i, 10));
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
    //draw the snake
    for (int l = 0; l < snake.size(); ++l) {
        move(snake[l].y, snake[l].x);
        addch(snakeBodyChar);
    }
    //write the points
    move(maxHeight - 1, 0);
    printw("%d", points);
    //draw the points
    move(growthItem.y, growthItem.x);
    addch(growthItemChar);
    refresh();
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

        moveSnake();
        if (direction == 'q') {
            break;
        }
        usleep(tick);
    }
}
