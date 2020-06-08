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
(wall - gatePair o, Immune wall - gatePair x)

(5) 점수 계산
1.몸의 최대길이
2.획득한 growth item,poison item의 수
3.게임중 사용한 gate의 수
4.게임시간(seconds)
위 4가지의 목표치를 만들고 해당 목표치 달성 시 게임 종료(이것 또한 몇개를 달성해야 종료할지는 알아서 정하기)
 */

#include <ctime>
#include <random>
#include <zconf.h>
#include "snake.h"
#include "ncurses.h"

/*
 * TODO List
 * - Implement Goal
 * - Implement gatePair
 * - Add Obstacle -> Same as add Stage
 * - Revise collision method -> get the Character of current snakeHead's coordinate
 * - add stage -> Revise Constructor
 * - popup Press any button to start when start game
 * - Implement scoreBoard (solved)
 * - Put item randomly (max item 3) (solved)
 * - Distinct snakeHead and snakeBody (solved)
 */


int getRandom(int min, int max) {
    std::random_device rd;
    std::mt19937 twister(rd());
    std::uniform_int_distribution<int> range(min, max);
    return range(twister);
}

snakePart::snakePart(int col, int row) {
    x = col;
    y = row;
}

snakePart::snakePart() {
    x = 0;
    y = 0;
}

SnakeClass::SnakeClass() {

    WINDOW *scoreBoard;
    WINDOW *Mission;
    WINDOW *startMenu;                              //   start menu완성 되면 삭제
    WINDOW *stage;                               
    
    startMenu = newwin(20, 20, 20, 20);             //   start menu완성 되면 삭제
    initscr();
    resize_term(100,100);                       
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    wattron(startMenu, 1);                                      //start menu완성 되면 삭제
    mvwprintw(startMenu, 10, 5, "Press Any Key To Start.");     //start menu완성 되면 삭제
    wrefresh(startMenu);                                        //start menu완성 되면 삭제
    getch();                                                    //start menu완성 되면 삭제
    

    // make new window of score board
    scoreBoard = newwin(8,37,4,27);
    //wborder(scoreBoard,'*','*','*','*','*','*','*','*');
    for(int i = 0;i<=37;i++){
        if(i == 0 || i == 36){
            mvwprintw(scoreBoard,0,i,"*");
            mvwprintw(scoreBoard,7,i,"*");
        }else{
            mvwprintw(scoreBoard,0,i,"-");
            mvwprintw(scoreBoard,7,i,"-");
        }
        
    }
    wrefresh(scoreBoard);

    //make new window of mission
    Mission = newwin(7,10,12,27);
    //wborder(Mission,'*','*','*','*','*','*','*','*');
    for(int i = 0;i<=9;i++){
        if(i == 0 || i == 9){
            mvwprintw(Mission,0,i,"*");
            mvwprintw(Mission,6,i,"*");
        }else{
            mvwprintw(Mission,0,i,"-");
            mvwprintw(Mission,6,i,"-");
        }
        
    }
    wrefresh(Mission);

    //new window of current stage
    stage = newwin(3,8,0,27);
    wborder(stage,'-','-','-','-','-','-','-','-');
    mvwprintw(stage,1,1,"stage%d",1);
    wrefresh(stage);

    nodelay(stdscr, true); // the program not wait until the user press a key
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    stageHeight = 25;
    stageWidth = 25;

    //init variables
    snakeLength = 3;
    growthCount = 0;
    poisonCount = 0;
    totalGrowth = 0;
    totalPoison = 0;
    cntGate = 0;
    snakeMaxLength = 3;
    missionGrowth = 5;
    missionPoison = 3;
    missionGate = 5;
    endScore = 5;


    // start init item location, gate location
    for (int m = 0; m < 2; ++m) {
        growthItems[m].x = -1;
        growthItems[m].y = -1;
        poisonItems[m].x = -1;
        poisonItems[m].y = -1;
        gatePair[m].x = -1;
        gatePair[m].y = -1;
        gatePair[m].doorX = -1;
        gatePair[m].doorY = -1;
    }
    // end init item location

    snakeHeadChar = '3';
    snakeBodyChar = '4';
    wallChar = '1';
    immuneWallChar = '2';
    growthItemChar = '*';
    poisonItemChar = 'x';
    gateChar = 'G';
    points = 0;
    tick = 150000; // Refresh Rate(Frequency)
    itemTick = 0;
    getGrowth = false;
    getPoison = false;
    meetGate = -1;
    direction = 'l';
    strcpy(scoreBoardChar, "Score Board");

    // draw initial snake
    for (int i = 0; i < 3; ++i) {
        snake.emplace_back(stageWidth / 2 + i, stageHeight / 2);
    }
    // end initial snake


    //draw the edge -> Will be upgraded draw the stage
    for (int x = 0; x < stageWidth; ++x) {
        for (int y = 0; y < stageHeight; ++y) {
            move(y, x);
            if (x == 0 || x == stageWidth - 1) {
                if (y == 0 || y == stageHeight - 1) {
                    addch(immuneWallChar);
                } else {
                    addch(wallChar);
                }
            } else if (y == 0 || y == stageHeight - 1) {
                if (x == stageWidth - 1) {
                    addch(immuneWallChar);
                } else {
                    addch(wallChar);
                }
            } else {
                addch(' ');
            }
        }
    }

    //Initial - draw the snake
    for (int l = 0; l < snake.size(); ++l) {
        move(snake[l].y, snake[l].x);
        addch(l == 0 ? snakeHeadChar : snakeBodyChar);
    }
    // end draw snake

    //draw initial items, gate
    putGrowth(0);
    putGrowth(1);
    putPoison(0);
    // end draw initial item, gate

    initBoard();
    refresh();
    //displayScore();
}

void SnakeClass::start() {
    while (true) {
        if (collision()) {
            move(stageWidth / 2 - 4, stageHeight / 2);
            printw("Game Over");
            break;
        }
        //displayScore();
        refreshSnake();
        if (direction == 'q') {
            break;
        }

        if (checkScore()) {
            move(stageWidth / 2 - 4, stageHeight / 2);
            printw("You Win!");
            break;
        }
        itemTick += 1;
        if (itemTick == 10) {
            itemTick = 0;
            for (int i = 0; i < 2; ++i) {
                if (growthItems[i].x != -1) {
                    putGrowth(i);
                }
                if (poisonItems[i].x != -1) {
                    putPoison(i);
                }
            }
        }
        usleep(tick);
    }
}

void SnakeClass::displayScore() const {
    
    
    //write the points
    move(6, stageHeight + 7);
    printw("%d", snakeLength);
    move(7, stageHeight + 7);
    printw("%d", totalGrowth);
    move(8, stageHeight + 7);
    printw("%d", totalPoison);
    move(9, stageHeight + 7);


    
    // for debug
    move(20, stageHeight + 13);
    printw("current growth count %d", growthCount);
    move(21, stageHeight + 13);
    printw("current poison count %d", poisonCount);
    for (int i = 0; i < 2; ++i) {
        move(i + 19, stageHeight + 13);
        printw("growthItems coordinate %d : %d, %d", i, growthItems[i].x, growthItems[i].y);
        move(i + 19 + 3, stageHeight + 13);
        printw("poisonItems coordinate %d : %d, %d", i, poisonItems[i].x, poisonItems[i].y);
    }
    move(24, stageHeight + 13);
    printw("gatePair[0] : %d, %d", gatePair[0].x, gatePair[0].y);
    move(25, stageHeight + 13);
    printw("gatePair[1] : %d, %d", gatePair[1].x, gatePair[1].y);
    // end for debug

    refresh();
}

bool SnakeClass::checkScore() const {
    return points == endScore;
}

void SnakeClass::initBoard() const {
    move(5, stageHeight + 4);
    addstr("Score Board");
    move(6, stageHeight + 4);
    addstr("B: 3 (Current Length)/(Max Length)");
    move(7, stageHeight + 4);
    addstr("+: 0 (Obtained Growth Items)");
    move(8, stageHeight + 4);
    addstr("-: 0 (Obtained Poison Items)");
    move(9, stageHeight + 4);
    addstr("G: 0 (Gate Used)");
    move(10, stageHeight + 4);
    addstr("T: 0 (Gate Used)");

    move(13, stageHeight + 4);
    addstr("Mission");
    move(14, stageHeight + 4);
    addstr("B:");
    move(14, stageHeight + 7);
    printw("%d", endScore);


    move(15, stageHeight + 4);
    addstr("+:");
    move(15, stageHeight + 7);
    printw("%d", missionGrowth);
    move(16, stageHeight + 4);
    addstr("-:");
    move(16, stageHeight + 7);
    printw("%d", missionPoison);

    move(17, stageHeight + 4);
    addstr("G:");
    move(17, stageHeight + 7);
    printw("%d", missionGate);

}

void SnakeClass::putGrowth(int whichGrowth) {
    move(growthItems[whichGrowth].y, growthItems[whichGrowth].x);
    addch(' ');
    char tempChar;
    while (true) {
        int tempX = getRandom(1, stageWidth - 3);
        int tempY = getRandom(1, stageHeight - 4);
        move(tempY, tempX);
        tempChar = inch();
        if (tempChar != ' ') {
            continue;
        }
        growthItems[whichGrowth].x = tempX;
        growthItems[whichGrowth].y = tempY;
        break;
    }
    move(growthItems[whichGrowth].y, growthItems[whichGrowth].x);
    addch(growthItemChar);
    growthCount += 1;
//    refresh();
}

void SnakeClass::putPoison(int whichPoison) {
    move(poisonItems[whichPoison].y, poisonItems[whichPoison].x);
    addch(' ');
    char tempChar;
    while (true) {
        int tempX = getRandom(1, stageWidth - 3);
        int tempY = getRandom(1, stageHeight - 4);
        tempChar = inch();
        if (tempChar != ' ') {
            continue;
        }
        poisonItems[whichPoison].x = tempX;
        poisonItems[whichPoison].y = tempY;
        break;
    }
    move(poisonItems[whichPoison].y, poisonItems[whichPoison].x);
    addch(poisonItemChar);
    poisonCount += 1;
//    refresh();
}

void SnakeClass::putGate() {
    if (gatePair->x != -1) {
        removeGate();
    }
    int gateIndex = 0;
    while (gateIndex < 2) {
        for (int i = 0; i < stageWidth; ++i) {
            for (int j = 0; j < stageHeight; ++j) {
                move(j, i);
                if (inch() == wallChar) {
                    if (getRandom(0, 50) == 1 && gateIndex < 2) {
                        gatePair[gateIndex].x = i;
                        gatePair[gateIndex].y = j;
                        addch(gateChar);
                        gateIndex += 1;
                    } else {
                        break;
                    }
                }
            }
        }
    } // end while
} // end putGate

void SnakeClass::findWayOut(int whichGate) {
    gatePart targetGate = gatePair[whichGate];
    if (direction == 'l') {
        move(targetGate.y, targetGate.x - 1);
        if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x - 1 > 0) {
            targetGate.doorX = targetGate.x - 1;
            targetGate.doorY = targetGate.y;
        } else {
            move(targetGate.y - 1, targetGate.x);
            if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.y - 1 > 0) {
                targetGate.doorX = targetGate.x;
                targetGate.doorY = targetGate.y - 1;
            } else {
                move(targetGate.y, targetGate.x + 1);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                    targetGate.x + 1 < stageWidth - 1) {
                    targetGate.doorX = targetGate.x + 1;
                    targetGate.doorY = targetGate.y;
                    direction = 'r';
                } else {
                    move(targetGate.y + 1, targetGate.x);
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y + 1;
                }
            }
        }
    } else if (direction == 'u') {
        move(targetGate.y - 1, targetGate.x);
        if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.y - 1 > 0) {
            targetGate.doorX = targetGate.x;
            targetGate.doorY = targetGate.y - 1;
        } else {
            move(targetGate.y, targetGate.x + 1);
            if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                targetGate.x + 1 < stageWidth - 1) {
                targetGate.doorX = targetGate.x + 1;
                targetGate.doorY = targetGate.y;
            } else {
                move(targetGate.y + 1, targetGate.x);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                    targetGate.y + 1 < stageHeight - 1) {
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y + 1;
                    direction = 'd';
                } else {
                    move(targetGate.y, targetGate.x - 1);
                    targetGate.doorX = targetGate.x - 1;
                    targetGate.doorY = targetGate.y;
                }
            }
        }
    } else if (direction == 'r') {
        move(targetGate.y, targetGate.x + 1);
        if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x + 1 < stageWidth - 1) {
            targetGate.doorX = targetGate.x + 1;
            targetGate.doorY = targetGate.y;
        } else {
            move(targetGate.y + 1, targetGate.x);
            if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                targetGate.y + 1 < stageHeight - 1) {
                targetGate.doorX = targetGate.x;
                targetGate.doorY = targetGate.y + 1;
            } else {
                move(targetGate.y, targetGate.x - 1);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x - 1 > 0) {
                    targetGate.doorX = targetGate.x - 1;
                    targetGate.doorY = targetGate.y;
                    direction = 'l';
                } else {
                    move(targetGate.y - 1, targetGate.x);
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y - 1;
                }
            }
        }
    } else if (direction == 'd') {
        move(targetGate.y + 1, targetGate.x);
        if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
            targetGate.y + 1 < stageHeight - 1) {
            targetGate.doorX = targetGate.x;
            targetGate.doorY = targetGate.y + 1;
        } else {
            move(targetGate.y, targetGate.x - 1);
            if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x - 1 > 0) {
                targetGate.doorX = targetGate.x - 1;
                targetGate.doorY = targetGate.y;
            } else {
                move(targetGate.y - 1, targetGate.x && targetGate.y - 1 > 0);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar) {
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y - 1;
                    direction = 'u';
                } else {
                    move(targetGate.y, targetGate.x + 1);
                    targetGate.doorX = targetGate.x + 1;
                    targetGate.doorY = targetGate.y;
                }
            }
        }
    }
    gatePair[whichGate] = targetGate;
}

void SnakeClass::removeGate() {
    for (int i = 0; i < 2; ++i) {

        move(gatePair[i].y, gatePair[i].x);
        addch(wallChar);
        gatePair[i].x = -1;
        gatePair[i].y = -1;
        gatePair[i].doorX = -1;
        gatePair[i].doorY = -1;
    }
}

bool SnakeClass::collision() {
    bool result = false;
    // check if snake is too short is collision with wall
    if (currentChar == wallChar || currentChar == immuneWallChar || currentChar == snakeBodyChar) {
        result = true;
    }
    if (snakeLength < 3) {
        result = true;
    }


    // get something?
    for (int j = 0; j < 2; ++j) {
        // get growth?
        if (snake[0].x == growthItems[j].x && snake[0].y == growthItems[j].y) {
            growthItems[j].x = -1;
            growthItems[j].y = -1;
            getGrowth = true;
            growthCount -= 1;
            if (growthCount == 0) {
                putGrowth(j);

            } else if (growthCount == 1 && getRandom(0, 1) == 0) {
                putGrowth(j);

            } else {
                putPoison(j);
            }

            points += 1;
            totalGrowth += 1;
            snakeLength += 1;
            displayScore();
            break;

            // get poison?
        } else if (snake[0].x == poisonItems[j].x && snake[0].y == poisonItems[j].y) {
            poisonItems[j].x = -1;
            poisonItems[j].y = -1;
            poisonCount -= 1;
            getPoison = true;
            if (poisonCount == 0) {
                putPoison(j);

            } else if (poisonCount == 1 && getRandom(0, 1) == 0) {
                putGrowth(j);

            } else {
                putPoison(j);
            }
            totalPoison += 1;
            points -= 1;
            snakeLength -= 1;
            displayScore();
            break;
            // meet gate?
        } else if (snake[0].x == gatePair[j].x && snake[0].y == gatePair[j].y) {
            meetGate = j;
            result = false;
            cntGate += 1;
            break;

        } else {
            getGrowth = false;
            getPoison = false;
            meetGate = -1;
        }
    } // end for
    return result;
}

void SnakeClass::refreshSnake() {
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
        if (meetGate == 0) {
            findWayOut(1);
            snake.insert(snake.begin(), snakePart(gatePair[1].doorX, gatePair[1].doorY));
        } else if (meetGate == 1) {
            findWayOut(0);
            snake.insert(snake.begin(), snakePart(gatePair[0].doorX, gatePair[0].doorY));
        } else {
            snake.insert(snake.begin(), snakePart(snake[0].x - 1, snake[0].y));
        }
    } else if (direction == 'r') {
        if (meetGate == 0) {
            findWayOut(1);
            snake.insert(snake.begin(), snakePart(gatePair[1].doorX, gatePair[1].doorY));
        } else if (meetGate == 1) {
            findWayOut(0);
            snake.insert(snake.begin(), snakePart(gatePair[0].doorX, gatePair[0].doorY));
        } else {
            snake.insert(snake.begin(), snakePart(snake[0].x + 1, snake[0].y));
        }
    } else if (direction == 'u') {
        if (meetGate == 0) {
            findWayOut(1);
            snake.insert(snake.begin(), snakePart(gatePair[1].doorX, gatePair[1].doorY));
        } else if (meetGate == 1) {
            findWayOut(0);
            snake.insert(snake.begin(), snakePart(gatePair[0].doorX, gatePair[0].doorY));
        } else {
            snake.insert(snake.begin(), snakePart(snake[0].x, snake[0].y - 1));
        }
    } else if (direction == 'd') {
        if (meetGate == 0) {
            findWayOut(1);
            snake.insert(snake.begin(), snakePart(gatePair[1].doorX, gatePair[1].doorY));
        } else if (meetGate == 1) {
            findWayOut(0);
            snake.insert(snake.begin(), snakePart(gatePair[0].doorX, gatePair[0].doorY));
        } else {
            snake.insert(snake.begin(), snakePart(snake[0].x, snake[0].y + 1));
        }
    }

    move(snake[0].y, snake[0].x);
    currentChar = inch();
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

    if (getRandom(0, 20) == 1) {
        putGate();
    }

    move(23, stageHeight + 13);
    printw("snake : %d, %d", snake[0].x, snake[0].y);
    move(24, stageHeight + 13);
    printw("current direction: %c", direction);
    getPoison = false;
    getGrowth = false;
    refresh();
}

SnakeClass::~SnakeClass() {
    nodelay(stdscr, false);
    getch();
    endwin();
}