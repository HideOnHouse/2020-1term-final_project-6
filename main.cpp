//
// Created by hideoncode on 5/16/20.
//
#include "snake.h"
#include "ncurses.h"

int main() {
    bool stageComplete=false;
    SnakeClass s = SnakeClass(1);
    stageComplete = s.start();
    if(!stageComplete)
        return 0;
    
    SnakeClass s2 = SnakeClass(2);
    stageComplete = s2.start();
    if(!stageComplete)
        return 0;


    SnakeClass s3 = SnakeClass(3);
    stageComplete = s3.start();
    if(!stageComplete)
        return 0;

    SnakeClass s4 = SnakeClass(4);
    stageComplete = s4.start();
    if(!stageComplete)
        return 0;
    
    return 0;
}