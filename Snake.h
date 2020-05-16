//
// Created by FURO on 2020-05-14.
//
#include "Map.h"
#include "Wall.h"
#include<ncurses.h>


#ifndef INC_2020_1TERM_FINAL_PROJECT_6_SNAKE_H
#define INC_2020_1TERM_FINAL_PROJECT_6_SNAKE_H

class Snake {
private:
    unsigned int size;
    int direction;
public:
    void move();

    /*
     * Change the direction depends on user keypress.
     */
    void changeDirection(int targetDirection);

    /*
     * If status of meetWall is 1 or 2, means game over
     * If status of meetWall is 3 or 4, means game over
     * If status of meetWall is 5 or 6, means update size
     * If status of meetWall is 7, means snake meet Gate;
     */
    void event(Wall meetWall);

    int getDirection(){
        return direction;
    }
    
        
    void setDirection(int dir){
        direction = dir;
    }
    
    /*
     * decrease size from tail
     */
    void decreaseSize();
        
    /*
     * increase size from head
     */
    void increaseSize();

    /*
    *  
    */
};


#endif //INC_2020_1TERM_FINAL_PROJECT_6_SNAKE_H
