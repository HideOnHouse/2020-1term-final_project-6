/*
 * Game Rule
 * (1)
1.진행방향 반대(tail)로 이동시 실패
2.snake는 벽을 통과 x, 자신의 몸도 통과x
3.head방향 이동은 일정시간에 의해 이동
(2) ITEM
1. growth item은 몸의 길이 증가(진행방향으로)
2. poison item은 감소(꼬리방향 감소) 단, 몸의 길이가 3보다 작아지면 실패
3.item의 출현은 snake body가 있지 않은 곳 임의의 위치에 출현, 출현 후 일정시간이 지나면 사라지고 다른곳에 나타남, 동시에 나타날 수 있는 item의 수는 최대 3개
(3)  GATE
1. gate는 두 개가 한 쌍. 한 번에 한 쌍만 나타난다.
2. gate는 겹치지 않는다.
3. gate는 임의의 벽에서 나타난다
4. gate를 통해 나갈 수 있는 진출로가 여러개이면 진행방향이 1순위가 진행방향이 막혀있으면 시계방향순으로 순위가 정해진다
5. gate의 출현 방법은 알아서 결정한다. 정해진게 없음.  ex)게임 시작 후 일정 시간 지나고 나오게 하기, 몸의 길이가 어느 정도 커지면 나오게 하기)
(4) WALL
1. 모든 벽은 snake가 통과x
1. 벽은 두 가지 종류 gate로 변할 수 있나 없나
(wall - gate o, Immune wall - gate x)
(5) 점수 계산
1.몸의 최대길이
2.획득한 growth item,poison item의 수
3.게임중 사용한 gate의 수
4.게임시간(seconds)
위 4가지의 목표치를 만들고 해당 목표치 달성 시 게임 종료(이것 또한 몇개를 달성해야 종료할지는 알아서 정하기)
 */

/*
 * TODO List
 * - Implement Goal
 * - Implement gate
 * - Add Obstacle -> Same as add Stage
 * - Revise collision method -> get the Character of current snakeHead's coordinate
 * - add stage -> Revise Constructor
 * - popup Press any button to start when start game
 * - Implement scoreBoard (solved)
 * - Put item randomly (max item 3) (solved)
 * - Distinct snakeHead and snakeBody (solved)
 */

/*
 *해결해야할 버그
 * - 아이템을 먹었을 때 해당 아이템이 제대로 작동하지 않는경우가 있는데, 이러면 총 Item개수가 줄어듬.
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

    snakeMaxLength=3;
    missionGrowth=5;
    missionPoision=3;
    missionGate=5;
    endScore = 10;

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
    points = 0;
    tick = 200000; // Refresh Rate(Frequency)
    getGrowth = false;
    getPoison = false;
    direction = 'l';
    srand(time(0));
    strcpy(scoreBoardChar, "Score Board");

    // draw initial snake
    for (int i = 0; i < 3; ++i) {
        snake.push_back(snakePart(screenWidth / 2 + i, screenHeight / 2));
    }
    // end draw initial snake


    //draw the edge -> Will be upgraded draw the stage
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
    // end draw edge

    //draw initial items
    putGrowth(0);
    putGrowth(1);
    putPoison(0);
    // end draw initial item

    initBoard();
    refresh();
    //displayScore();
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

        if(checkScore()==true) {
            move(screenWidth / 2 - 4, screenHeight / 2);
            printw("Game Over");
            break;
        }
        usleep(tick);
    }
}

void SnakeClass::displayScore() {

    //write the points
    move(3, screenHeight + 13);
    printw("%d", snakeLength/snakeMaxLength);
    move(4, screenHeight + 13);
    printw("%d", totalGrowth);
    move(5, screenHeight + 13);
    printw("%d", totalPoison);
    move(7, screenHeight + 13);
    refresh();
}

bool SnakeClass::checkScore() {
    // 미션달성시 true반환하여 게임 끝내기 
    if(points==endScore){//일단 목표 growth=5개 달성시 종료 
        return true;
    }
    else{
        return false;
    }
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
    move(7, screenHeight + 10);
    addstr("T: 0 (Gate Used)");
    
    move(9, screenHeight + 10);
    addstr("Mission");
    move(10, screenHeight + 10);
    addstr("B:");

    move(10, screenHeight + 13);
    printw("%d", points);

    move(11, screenHeight + 10);//Growth 미션 개수 
    addstr("+:");
    move(11, screenHeight + 13);
    printw("%d", missionGrowth);
    
    move(12, screenHeight + 10); //포이즌 미션 개수 
    addstr("-:");
    move(12, screenHeight + 13);
    printw("%d", missionPoision);
    
    move(13, screenHeight + 10);//게이트 미션 개수
    addstr("G:");
    move(13, screenHeight + 13);
    printw("%d", missionGate);
    
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
        srand((unsigned int)time(NULL));
        // get growth?
        if (snake[0].x == growthItems[j].x && snake[0].y == growthItems[j].y) {
            growthItems[j].x = -1;growthItems[j].y = -1;
            getGrowth = true;
            growthCount-=1;
            if(growthCount==0){
                putGrowth(j);
                
            }
            else if(growthCount == 1 && rand()%2 == 1){
                putGrowth(j);
                
            }
            else{
                putPoison(j);
                
            }
            
            points += 1;
            totalGrowth += 1;
            snakeLength += 1;
            displayScore();
            break;

            // get poison?
        } else if (snake[0].x == poisonItems[j].x && snake[0].y == poisonItems[j].y) {
            poisonItems[j].x = -1;poisonItems[j].y = -1;
            poisonCount-=1;
            getPoison = true;
            if(poisonCount==0){
                putPoison(j);
                
            }
            else if(poisonCount == 1 && rand()%2 == 0){
                putGrowth(j);
               
            }
            else{
                putPoison(j);
               
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
    if(snakeLength>=snakeMaxLength){
        snakeMaxLength=snakeLength; // snake 최대길이 설정 
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

SnakeClass::~SnakeClass() {
    nodelay(stdscr, false);
    getch();
    endwin();
}