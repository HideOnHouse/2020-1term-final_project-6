//
// Created by hideoncode on 5/16/20.
//
#include "snake.h"
#include "ncurses.h"

int main() {
    // WINDOW *startMenu;
    // startMenu = newwin(20, 20, 20, 20);
    // initscr();
    // start_color();
    // init_pair(1, COLOR_WHITE, COLOR_BLACK);
    // wattron(startMenu, 1);
    // mvwprintw(startMenu, 10, 5, "Press Any Key To Start.");
    // wrefresh(startMenu);
    // getch();
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