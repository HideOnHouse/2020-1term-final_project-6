//
// Created by hideoncode on 5/16/20.
//
#include "snake.h"
#include "ncurses.h"
int main() {
    WINDOW *startMenu;
    startMenu = newwin(20, 20, 20, 20);
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wattron(startMenu, 1);
    mvwprintw(startMenu, 10, 5, "Press Any Key To Start.");
    wrefresh(startMenu);
    getch();
    SnakeClass s;
    s.start();
    return 0;
}