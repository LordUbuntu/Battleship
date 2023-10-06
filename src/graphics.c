#include <ncurses.h>
#include "data.h"
#pragma once


#define SHIP_CHAR '#'


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
                        wvline(win, SHIP_CHAR, s.back.y - s.front.y);
                } else {
                        // draw horizontal line
                        whline(win, SHIP_CHAR, s.back.x - s.front.x);
                }
        }
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
