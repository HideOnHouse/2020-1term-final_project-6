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
    SnakeClass s = SnakeClass(1);
    s.start();
    SnakeClass s2 = SnakeClass(2);
    s2.start();
    SnakeClass s3 = SnakeClass(3);
    s3.start();
    SnakeClass s4 = SnakeClass(4);
    s4.start();
    return 0;
}