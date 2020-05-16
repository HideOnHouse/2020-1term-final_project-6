#include "Snake.h"
#include<ncurses.h>

void Snake::move(){
    initscr();
    int ch;
    keypad(stdscr,TRUE);
    
    while(true){
        ch = getch();
        if(getDirection() == KEY_LEFT && ch == KEY_RIGHT){break;}
        else if(getDirection() == KEY_RIGHT && ch == KEY_LEFT){break;}
        else if(getDirection() == KEY_UP && ch == KEY_DOWN){break;}
        else if(getDirection() == KEY_DOWN && ch == KEY_UP){break;}


        if(getDirection() != ch){
            changeDirection(ch);
        }
        if(ch == KEY_LEFT){
            printw("left");
            setDirection(ch);
        }
        else if(ch==KEY_UP){
            printw("up");
            setDirection(ch);
        }
        else if(ch==KEY_RIGHT){
            printw("right");
            setDirection(ch);
        }
        else if(ch==KEY_DOWN){
            printw("down");
            setDirection(ch);   
        }
        else if(ch==KEY_F(1)){
            printw("game over");
            break;
        }
    }
}

void Snake::changeDirection(int targetDirection){
    setDirection(targetDirection);
}

void Snake::event(Wall meetWall){

}

void Snake::decreaseSize(){

}

void Snake::increaseSize(){

}

int main(){
    Snake sn = Snake();
    sn.move();
}