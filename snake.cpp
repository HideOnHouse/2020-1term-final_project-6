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
#include <locale.h>
#include "snake.h"
#include "stageClass.h"
#include "ncurses.h"

/*
 * TODO List
 * - Implement Goal
 * - Implement gatePair
 * - Add Obstacle -> Same as add Stage
 * - Revise collision method -> get the Character of current snakeHead's coordinate
 * - add stageClass -> Revise Constructor
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

SnakeClass::SnakeClass(int whichStage) {

    WINDOW *startMenu;          //start menu완성 되면 삭제
    WINDOW *Mission;
    WINDOW *stage;

    stageClass currentStage(whichStage);

    setlocale(LC_ALL,"");
    initscr();
    resize_term(82, 82);
    start_color();
    //bkgd(COLOR_PAIR(1));
    //attron(COLOR_PAIR(1));
    init_pair(1, COLOR_WHITE, COLOR_BLACK);             //처음 press any button to start game의 팔레트 앞 : 글씨색깔, 뒤 : 배경색깔
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);            //점수판의 색깔 팔레트 앞 : 글씨색깔, 뒤 : 배경색깔
    
    //startMenu = newwin(7, 2, 2, 15);
    //attron(COLOR_PAIR(1));                              //start menu완성 되면 삭제
    mvprintw(0, 1, "Press Any Key ");                  //start menu완성 되면 삭제
    mvprintw(1, 1, "To Start.");                       //start menu완성 되면 삭제
    //attroff(COLOR_PAIR(1));                             //start menu완성 되면 삭제
    //wrefresh(startMenu);                                          //start menu완성 되면 삭제
    getch();
    //delwin(startMenu);
    //endwin();

    nodelay(stdscr, true); // the program not wait until the user press a key
    keypad(stdscr, true);
    noecho();
    curs_set(0);

    stageHeight = currentStage.stageHeight;
    stageWidth = currentStage.stageWidth;

    //init variables
    snakeLength = 3;
    growthCount = 0;
    poisonCount = 0;
    totalGrowth = 0;
    totalPoison = 0;
    cntGate = 0;
    gameTimer = 0;
    missionGrowth = currentStage.missionGrowth;
    missionPoison = currentStage.missionPoison;
    missionGate = currentStage.missionGate;
    endScore = currentStage.endScore;
    itemTick = currentStage.itemTick;
    nextStage = whichStage;

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
    getGrowth = false;
    getPoison = false;
    meetGate = -1;
    direction = 'l';
    strcpy(scoreBoardChar, "Score Board");

    // draw initial snake
    for (int i = 0; i < 3; ++i) {
        snake.emplace_back(stageWidth / 4 + i, stageHeight / 3);
    }
    // end initial snake


    //draw the edge -> Will be upgraded draw the stageClass
    switch (whichStage) {
        case 1:
            for (int x = 0; x < stageWidth; ++x) {
                for (int y = 0; y < stageHeight; ++y) {
                    move(y, x);
                    //addch("\2B1B");
                    //if(currentStage.stage1[x][y] == '1') printw("U+0CF2");
                    //else if(currentStage.stage1[x][y] == '2')printw("\2B1C");
                    addch(currentStage.stage1[x][y]);
                }
            }
            break;
        case 2:
            for (int x = 0; x < stageWidth; ++x) {
                for (int y = 0; y < stageHeight; ++y) {
                    move(y, x);
                    addch(currentStage.stage2[x][y]);
                }
            }
            break;
        case 3:
            for (int x = 0; x < stageWidth; ++x) {
                for (int y = 0; y < stageHeight; ++y) {
                    move(y, x);
                    addch(currentStage.stage3[x][y]);
                }
            }
            break;
        case 4:
            for (int x = 0; x < stageWidth; ++x) {
                for (int y = 0; y < stageHeight; ++y) {
                    move(y, x);
                    addch(currentStage.stage4[x][y]);
                }
            }
            break;
    }

    //Initial - draw the snake
    for (int l = 0; l < snake.size(); ++l) {
        move(snake[l].y, snake[l].x);
        addch(l == 0 ? snakeHeadChar : snakeBodyChar);
    }
    // end draw snake

    //make new window of mission
    Mission = newwin(7, 10, 14, 36);
    box(Mission,0,0);
    // wattron(Mission, COLOR_PAIR(1));
    // wborder(Mission, '-', '-', '-', '-', '+', '+', '+', '+');
    // wattroff(Mission, COLOR_PAIR(1));
    wattron(Mission, COLOR_PAIR(2));
    mvwprintw(Mission, 1, 2, "Mission");
    mvwprintw(Mission, 2, 2, "B:%d", endScore);
    mvwprintw(Mission, 3, 2, "+:%d", missionGrowth);
    mvwprintw(Mission, 4, 2, "-:%d", missionPoison);
    mvwprintw(Mission, 5, 2, "G:%d", missionGate);
    wattroff(Mission, COLOR_PAIR(2));
    wrefresh(Mission);

    //new window of current stage
    stage = newwin(4, 10, 1, 36);
    box(stage,0,0);
    // wattron(stage, COLOR_PAIR(1));
    // wborder(stage, '-', '-', '-', '-', '+', '+', '+', '+');
    // wattroff(stage, COLOR_PAIR(1));
    wattron(stage, COLOR_PAIR(2));
    mvwprintw(stage, 1, 1, "stage");
    switch (whichStage) {
        case 1:
            mvwprintw(stage, 2, 1, "Class %d", 1);
            break;
        case 2:
            mvwprintw(stage, 2, 1, "Class %d", 2);
            break;
        case 3:
            mvwprintw(stage, 2, 1, "Class %d", 3);
            break;
        case 4:
            mvwprintw(stage, 2, 1, "Class %d", 4);
            break;

    }
    wattroff(stage, COLOR_PAIR(2));
    wrefresh(stage);

    //draw initial items, gate
    putGrowth(0);
    putGrowth(1);
    putPoison(0);
    // end draw initial item, gate

    initBoard();
    refresh();
    //displayScore();
}

bool SnakeClass::start() {
    while (true) {
        if (collision()) {
            move(stageWidth / 2 - 4, stageHeight / 2);
            printw("Game Over");
            usleep(tick);
            return false;
        }
        //displayScore();
        refreshSnake();
        if (direction == 'q') {
            break;
        }

        if (checkScore()) {
            usleep(tick);
            return true;
        }

        // // Check time per item
        // for(int i = 0;i<2;i++){
        //     if(growthItems[i].x != -1) growthItems[i].time += 1;
        //     if(poisonItems[i].x != -1) poisonItems[i].time += 1;
        // }

        // for(int i = 0;i<2;++i){
        //     if(growthItems[i].time == 35){
        //         if (growthItems[i].x != -1){
        //             growthCount -= 1;
        //             putGrowth(i);  
        //         }
        //     }
        //     if(poisonItems[i].time==35){
        //         if (poisonItems[i].x != -1){
        //             poisonCount -= 1;
        //             putPoison(i);
        //         }    
        //     }
        // }
        // if(poisonCount==2 && (poisonItems[0].x) == -1){
        //     poisonCount -= 1;
        //     putPoison(0);
        // }
        // if(poisonCount==2 && (poisonItems[1].x) == -1){
        //     poisonCount -= 1;
        //     putPoison(1);
        // }
        // if(growthCount==2 && (growthItems[0].x) == -1){
        //     growthCount -= 1;
        //     putGrowth(0);
        // }
        // if(growthCount==2 && (growthItems[1].x) == -1){
        //     growthCount -= 1;
        //     putGrowth(1);
        // }

        if (growthCount == 0) putGrowth(0);
        itemTick += 1;
        if (itemTick == 35) {
            itemTick = 0;
            for (int i = 0; i < 2; ++i) {
                if (growthItems[i].x != -1) {
                    growthCount -= 1;
                    putGrowth(i);
                }
                if (poisonItems[i].x != -1) {
                    poisonCount -= 1;
                    putPoison(i);
                }
            }
        }
        if (poisonCount == 2 && (poisonItems[0].x) == -1) {
            poisonCount -= 1;
            putPoison(0);
        }
        if (poisonCount == 2 && (poisonItems[1].x) == -1) {
            poisonCount -= 1;
            putPoison(1);
        }
        if (growthCount == 2 && (growthItems[0].x) == -1) {
            growthCount -= 1;
            putGrowth(0);
        }
        if (growthCount == 2 && (growthItems[1].x) == -1) {
            growthCount -= 1;
            putGrowth(1);
        }

        // make new window of score board
        WINDOW *scoreBoard;
        scoreBoard = newwin(8, 37, 5, 36);
        box(scoreBoard,0,0);
        // wattron(scoreBoard, COLOR_PAIR(1));
        // wborder(scoreBoard, '-', '-', '-', '-', '+', '+', '+', '+');
        // wattroff(scoreBoard, COLOR_PAIR(1));
        wattron(scoreBoard, COLOR_PAIR(2));
        mvwprintw(scoreBoard, 1, 2, "scoreboard");
        mvwprintw(scoreBoard, 2, 2, "B: %d (Current Length)/(Max Length)", snakeLength);
        mvwprintw(scoreBoard, 3, 2, "+: %d (Obtained Growth Items)", totalGrowth);
        mvwprintw(scoreBoard, 4, 2, "-: %d (Obtained Poison Items)", totalPoison);
        mvwprintw(scoreBoard, 5, 2, "G: %d (Gate Used)", missionGate);
        mvwprintw(scoreBoard, 6, 2, "T: 0 (Gate Used)");
        wattroff(scoreBoard, COLOR_PAIR(2));
        wrefresh(scoreBoard);
        gameTimer += 1;
        usleep(tick);
    }

    return false;
}

void SnakeClass::displayScore() const {
    // for debug
    // move(29, stageHeight + 13);
    // printw("current growth count %d", growthCount);
    // move(30, stageHeight + 13);
    // printw("current poison count %d", poisonCount);
    // for (int i = 0; i < 2; ++i) {
    //     move(i + 22, stageHeight + 13);
    //     printw("growthItems coordinate %d : %d, %d", i, growthItems[i].x, growthItems[i].y);
    //     move(i + 22 + 3, stageHeight + 13);
    //     printw("poisonItems coordinate %d : %d, %d", i, poisonItems[i].x, poisonItems[i].y);
    // }
    // move(27, stageHeight + 13);
    // printw("gatePair[0] : %d, %d", gatePair[0].x, gatePair[0].y);
    // move(28, stageHeight + 13);
    // printw("gatePair[1] : %d, %d", gatePair[1].x, gatePair[1].y);

    // refresh();
}

bool SnakeClass::checkScore() const {
    if(points == endScore || cntGate == missionGate || totalGrowth == missionGrowth || totalPoison == missionPoison || gameTimer == 20000000000){
        
        if(nextStage != 4){
            nodelay(stdscr,false);

            init_pair(3, COLOR_BLUE, COLOR_BLACK);  //congratulation you win 의 팔레트 앞 : 글씨색깔, 뒤 : 배경색깔
            init_pair(4, COLOR_CYAN,COLOR_BLACK);   //press any key to play stage의 팔레트 앞 : 글씨색깔, 뒤 : 배경색깔
            attron(COLOR_PAIR(3));
            
            mvprintw(stageHeight/2-4,2,"congratulation!!!!");
            mvprintw(stageHeight/2-3,2,"You Win!!");
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(4));
            mvprintw(stageHeight/2,2,"press any button");
            mvprintw(stageHeight/2+1,2,"to play stage%d",nextStage+1);
            
            getch();
            attroff(COLOR_PAIR(4));
            nodelay(stdscr,true);
            return true;
        }
        else{
            attron(COLOR_PAIR(4));
            mvprintw(stageHeight/2,2,"congratulation!!!!");
            mvprintw(stageHeight/2+1,2,"You're a KING");
            mvprintw(stageHeight/2+2,2,"GREAT!!!!");
            attroff(COLOR_PAIR(4));
            return true;
        }
    }
    return points == endScore;
    
}

void SnakeClass::initBoard() const {
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
        growthItems[whichGrowth].time = 0;
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
        move(tempY, tempX);
        tempChar = inch();
        if (tempChar != ' ') {
            continue;
        }
        poisonItems[whichPoison].x = tempX;
        poisonItems[whichPoison].y = tempY;
        poisonItems[whichPoison].time = 0;
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
                move(targetGate.y + 1, targetGate.x);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                    targetGate.y + 1 < stageHeight - 1) {
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y + 1;
                } else {
                    if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                        targetGate.x + 1 < stageWidth - 1){
                        targetGate.doorX = targetGate.x + 1;
                        targetGate.doorY = targetGate.y;
                        direction = 'r';
                    }
                    
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
                move(targetGate.y, targetGate.x - 1);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x - 1 > 0) {
                    targetGate.doorX = targetGate.x - 1;
                    targetGate.doorY = targetGate.y;
                } else {
                    move(targetGate.y + 1, targetGate.x);
                    if(inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                        targetGate.y + 1 < stageHeight - 1){
                        targetGate.doorX = targetGate.x;
                        targetGate.doorY = targetGate.y + 1;
                        direction = 'd';
                    }
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
                move(targetGate.y - 1, targetGate.x && targetGate.y - 1 > 0);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.y - 1 > 0) {
                    targetGate.doorX = targetGate.x;
                    targetGate.doorY = targetGate.y - 1;
                } else {
                    move(targetGate.y, targetGate.x - 1);
                    if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && targetGate.x - 1 > 0){
                        targetGate.doorX = targetGate.x - 1;
                        targetGate.doorY = targetGate.y;
                        direction = 'l';
                    }
                    
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
            move(targetGate.y, targetGate.x + 1);
            if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar &&
                targetGate.x + 1 < stageWidth - 1) {
                targetGate.doorX = targetGate.x + 1;
                targetGate.doorY = targetGate.y;
            } else {
                move(targetGate.y, targetGate.x - 1);
                if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar && 
                    targetGate.x - 1 > 0) {
                    targetGate.doorX = targetGate.x - 1;
                    targetGate.doorY = targetGate.y;
                } else {
                    move(targetGate.y - 1, targetGate.x && targetGate.y - 1 > 0);
                    if (inch() != wallChar && inch() != immuneWallChar && inch() != gateChar){
                        targetGate.doorX = targetGate.x;
                        targetGate.doorY = targetGate.y - 1;
                        direction = 'u';
                    }
                    
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

void SnakeClass::checkGate() {
    for (const auto &pair : gatePair) {
        move(pair.y, pair.x);
        if (inch() != gateChar) {
            addch(gateChar);
        }
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
        } else if (inch() != wallChar) {
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
    move(25, stageHeight + 13);
    printw("timer : %d", gameTimer);
    move(26, stageHeight + 13);
    printw("use gate : %d", cntGate);
    getPoison = false;
    getGrowth = false;
    refresh();
}

SnakeClass::~SnakeClass() {
    nodelay(stdscr, false);
    getch();
    endwin();
}