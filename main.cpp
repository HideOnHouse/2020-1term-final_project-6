//
// Created by hideoncode on 5/14/20.
//

#include <ncurses.h>

int main() {
    initscr();
    printw("Hello World!");
    refresh();
    getch();
    endwin();
    return 0;
}
