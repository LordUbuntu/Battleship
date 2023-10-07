#include <ncurses.h>
#include "data.h"
#pragma once


#define WATER '~'
#define SHIP '#'
#define HIT '*'
#define MISS '@'
#define DEFAULT_MAP { \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},\
}


void init_ncurses(void) {
        initscr();
        curs_set(FALSE);
        noecho();
        cbreak();
        start_color();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
}


int render_ships(WINDOW *win, ship *ships) {
        // iterate over each ship, drawing its position relative to (0,0)
        for (int i = 0; i < NUM_SHIPS; i++) {
                ship s = ships[i];
                // position cursor to draw ship
                wmove(win, s.front.y + 1, s.front.x + 1);
                // determine orientation
                if (s.back.x - s.front.x == 0) {
                        // draw vertical line
                        wvline(win, SHIP, s.back.y - s.front.y);
                } else if (s.back.y - s.front.y == 0) {
                        // draw horizontal line
                        whline(win, SHIP, s.back.x - s.front.x);
                } else {
                        return 1;
                }
        }
        wrefresh(win);
        return 0;
}


int render_map(WINDOW *win, map m) {
        for (int i = 0; i < 10; i++)
                for (int j = 0; j < 10; j++)
                        mvwaddch(win, i + 1, j + 1, m[i][j]);
        wrefresh(win);
        return 0;
}


void stop_ncurses(void) {
        clear();
        curs_set(TRUE);
        echo();
        nocbreak();
        endwin();
}
