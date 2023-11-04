#include <ncurses.h>
#include "data.h"
#pragma once


// characters and colours
#define WATER '~'
#define SHIP '#'
#define HIT '*'
#define MISS '@'
enum COLOR {
        COLOR_WATER = 0,
        COLOR_SHIP,
        COLOR_FIRE,
        COLOR_SPLASH,
};
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


void help(void) {
        // this is fine because the help page data is static
        clear();
        refresh();
        WINDOW *help = newwin(LINES, COLS, 0, 0);
        int input = 0;
        while (true) {
                if (input == 'q')
                        break;
                mvwprintw(help, 1, 1, "The goal of battleship is to sink all");
                mvwprintw(help, 2, 1, "of your opponent's pieces before they");
                mvwprintw(help, 3, 1, "sink yours.");

                mvwprintw(help, 5, 1, "First, place your pieces on your board");
                mvwprintw(help, 6, 1, "using the arrow keys, and ENTER to");
                mvwprintw(help, 7, 1, "confirm your placement. Ensure that");
                mvwprintw(help, 8, 1, "all pieces are not overlapping and");
                mvwprintw(help, 9, 1, "are not out of bounds.");
                mvwprintw(help, 10, 1, "Next, the game starts and you both");
                mvwprintw(help, 11, 1, "take turns using the arrow keys to");
                mvwprintw(help, 12, 1, "select an open spot on the opponent");
                mvwprintw(help, 13, 1, "board, and firing a volley with ENTER.");
                
                mvwprintw(help, 15, 1, "To return to the menu, press 'q'");

                wrefresh(help);

                input = getch();
        }
        delwin(help);
        erase();
        refresh();
}


void render_board(WINDOW *win, map board);


int render_ship(WINDOW *win, ship *s) {
        // position cursor to draw ship, note +1 offset to stay in border
        wmove(win, s->front.y + 1, s->front.x + 1);

        // determine orientation
        if (s->back.x - s->front.x == 0) {
                // draw vertical line
                wvline(win, SHIP, s->back.y - s->front.y + 1);
        } else if (s->back.y - s->front.y == 0) {
                // draw horizontal line
                whline(win, SHIP, s->back.x - s->front.x + 1);
        } else {
                // bizzare unalignment, shouldn't happen!
                return ERR;
        }
        wrefresh(win);
        return OK;
}


// WARN: may be depreciated
int render_ships(WINDOW *win, ship ships[static NUM_SHIPS]) {
        // iterate over each ship, drawing its position relative to (0,0)
        for (int i = 0; i < NUM_SHIPS; i++) {
                ship s = ships[i];
                int status = render_ship(win, &s);
                if (status)
                        return status;
        }
        return OK;
}


void stop_ncurses(void) {
        clear();
        curs_set(TRUE);
        echo();
        nocbreak();
        endwin();
}
