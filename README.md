# 2020-1st semester snake game project Team-6
2020학년도 1학기 기말프로젝트 C++ 언어로 Snake Game 만들기를 위한 레포지토리입니다.

## To-Do
> 1. Implement Gate
> 2. Implement Goal
> 3. Add stageClass by revise constructor, stageClass can be represented as 2-dimension array
> 4. Revise collision method -> get the Character of current snakeHead's coordinate
> 5. Append "Press any button to start when start game"
> 6. Remove member variable snakeLength
> 7. Clarify growthCount, poisonCount modification
> 8. CHANGE MOVESNAKE FUNCTION (WHEN USER INPUT OPPOSITE TO CURRENT DIRECTION)
## Table of Contents
- [1. Contributor](#1-contributor)
- [2. Version and Compile](#2-version-and-compile)
- [3. How to use](#3-how-to-use)
- [4. Game Rule](#4-game-rule)
    - [4.1. Snake](#41-snake)
    - [4.2. Item](#42-item)
    - [4.3. Wall](#43-wall)
        - [4.3.1. Normal Wall](#431-normal-wall)
        - [4.3.2. Immune Wall](#432-immune-wall)
        - [4.3.3. Gate](#433-gatePair)
    - [4.4. Score](#44-score)
- [5. About SnakeClass](#5-about-class)
    - [5.1. Constructor](#51-constructor)
    - [5.2. displayScore()](#52-displayscore)
    - [5.3. checkScore()](#53-checkscore)
    - [5.4. initBoard()](#54-initboard)
    - [5.5. putGrowth()](#55-putgrowth)
    - [5.6. putPoison()](#56-putpoison)
    - [5.7. collision()](#57-collision)
    - [5.8. refreshSnake()](#58-movesnake)
    - [5.9. Destructor](#59-destructor)

## 1. Contributor
- 차영호, 양재원, 허진우
## 2. Version and Compile
- Version: C++ 14
- Compile command: `g++ -std=c++14 -o test.out main.cpp snake.cpp stageClass.cpp -lncurses`
## 3. How to use
- Can run Snake game by using compiles file `test.out`
## 4. Game Rule
### 4.1. Snake
- 머리를 일정시간마다 사용자가 입력한 방향으로 나아간다
- 머리와 몸통(꼬리 포함)이 부딪히면 게임 종료
- 현재 진행방향과 반대로 이동시 게임 종료
- Normal Wall, Immune wall과 부딪히면 게임 종료
### 4.2. Item
- Growth Item은 몸의 길이가 증가한다 (진행방향으로)
- Poison Item은 몸의 길이가 감소한다 (꼬리부터)
- Poison으로 인해 몸의 길이가 3미만이 되면 게임 종료
### 4.3. Wall
#### 4.3.1. Normal Wall
- Snake가 통과할 수 없다
- Gate가 생성될 수 있다
#### 4.3.2. Immune Wall
- Snake가 통과할 수 없다
- Gate가 생성될 수 없다
#### 4.3.3. Gate
- 두 개가 한 쌍, 한 번에 한 쌍만 나타난다.
- Gate는 서로 겹치지 않는다
- Gate는 임의의 Normal Wall에서 나타난다
- Gate의 출구가 여러개이면 
     1순위 : 진행방향
     2순위 : 기존 진행방향의 시계방향
     3순위 : 기존 진행방향의 반시계 방향
     4순위 : 기존 진행방향의 반대방향
- Gate는 Length가 7 이상일때 나타난다
### 4.4. Score
- Snake의 길이가 15에 도달하면 다음 스테이지로
- Growth Item을 20개 획득하면 다음 스테이지로
- Poison Item을 5개 획득하면 게임 종료
- Gate를 5번 사용하면 게임 종료
- 2분을 버티면 다음 스테이지로
## 5. About Class
### 5.1. Constructor
`initscr()`: ncurses에서 스크린 사용 선언  
`nodelay(stdscr, true)`: stdscr(Standard Screen)은 사용자의 입력을 기다리지 않음  
`keypad(stdscr, true)`: stdscr은 사용자의 키패드 입력을 받아온다  
`snakeLength`: snake의 길이를 나타내는 변수이지만, snake.size()로 대체할 예정

```
    // start init item location
    for (int m = 0; m < 2; ++m) {
        growthItems[m].x = 0;
        growthItems[m].y = 0;
        poisonItems[m].x = 0;
        poisonItems[m].y = 0;
    }
    // end init item location
```
여기에서는 SnakePart Struct가 사용되는데, ncurses를 쉽게 사용하기 위한, 좌표를 가지고 있는 Block이다
SnakePart는 snake, item의 생성, 변경에 사용된다. growthItem, poisonItem은 SnakePart로 구성된 2 크기의 array
```
    //draw the edge -> Will be upgraded draw the stageClass
    for (int j = 0; j < stageWidth - 1; ++j) {
        move(stageHeight - 2, j);
        addch(wallChar);
    }
    for (int k = 0; k < stageHeight - 1; ++k) {
        move(k, stageWidth - 2);
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
```
스테이지를 그리는 부분이다. 아직 immune Wall은 구현하지 못했다
Constructor에서 2차원 배열(Stage)를 받아와서 그리는 걸로 업드레이드 할 예정
### 5.2. void displayScore()
Description deferred.
### 5.3. void checkScore()
Description deferred.
### 5.4. void initBoard()
Description deferred.
### 5.5. void putGrowth()
- snake, item, wall과 겹치지 않는 좌표를 생성한 후에(겹치지 않을 때까지) 해당하는 좌표에 growthItemChar 출력
- growthCount를 1 증가시킨다
- `refresh()`함수 호출
### 5.6. void putPoison()
- snake, item, wall과 겹치지 않는 좌표를 생성한 후에(겹치지 않을 때까지) 해당하는 좌표에 poisonItemChar 출력
- poisonCount를 1 증가시킨다
- `refresh()`함수 호출
### 5.7. bool collision()
- 게임이 종료되는 상태가 발생했는지, 아이템을 얻었는지 검사하는 함수
- 아이템을 검사할때, growthItems, poisonItems 원소들의 모든 좌표를 검사한다
- growthCount, poisonCount부분이 굉장히 복잡한데, 수정이 시급
- 멤버 변수 getGrowth, getPoison을 변경한다
- 게임이 종료되는 상태가 발생하면 true, 그렇지 않으면 false를 리턴한다
### 5.8. void refreshSnake()
- 유저의 인풋을 받아 방향을 결정한다
- 현재 진행방향의 반대로 입력하면 입력을 무시하지만, 게임 룰에 위배된다
- getGrowth, getPoison을 여기에서 검사하고, 포션의 행동을 실행시킨다
### 5.9. Destructor
- nodelay -> false
- end using window
