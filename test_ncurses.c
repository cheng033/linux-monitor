// src/test_ncurses.c
#include <ncurses.h>
int main() {
    initscr();
    printw("Hello ncurses!\n");
    refresh();
    getch();
    endwin();
    return 0;
}

