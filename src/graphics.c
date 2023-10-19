#include <ncurses.h>
#include "data.h"
#pragma once


// status codes
#define OK 0
#define ERR 1
// characters and colours
#define WATER '~'
#define SHIP '#'
#define HIT '*'
#define MISS '@'
// TODO: define enum for colours
// TODO: define and init colours


void init_ncurses(void) {
        initscr();
        curs_set(FALSE);
        noecho();
        cbreak();
        start_color();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
}


int render_ship(WINDOW *win, ship *s) {
        // position cursor to draw ship, note +1 offset to stay in border
        wmove(win, s.front.y + 1, s.front.x + 1);

        // determine orientation
        if (s.back.x - s.front.x == 0) {
                // draw vertical line
                wvline(win, SHIP, s.back.y - s.front.y);
        } else if (s.back.y - s.front.y == 0) {
                // draw horizontal line
                whline(win, SHIP, s.back.x - s.front.x);
        } else {
                // bizzare unalignment, shouldn't happen!
                return ERR;
        }
        wrefresh(win);
        return OK;
}


// WARN: may be depreciated
int render_ships(WINDOW *win, ship *ships) {
        // iterate over each ship, drawing its position relative to (0,0)
        for (int i = 0; i < NUM_SHIPS; i++) {
                ship s = ships[i];
                render_ship(win, &s);
        }
}


void stop_ncurses(void) {
        clear();
        curs_set(TRUE);
        echo();
        nocbreak();
        endwin();
}
