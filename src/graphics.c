#include <ncurses.h>
#include "data.h"


#define SHIP_CHAR '#'


int
init_ncurses(void)
{
        initscr();
        curs_set(FALSE);
        noecho();
        cbreak();
        start_color();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
        return 0;
}


int
render_ships(ship *ships) {
        // iterate over each ship, drawing its position relative to (0,0)
        for (int i = 0; i < NUM_SHIPS; i++) {
                ship s = ships[i];
                // position cursor to draw ship
                move(s.front.y, s.front.x);
                // determine orientation
                if (s.back.x - s.front.x == 0) {
                        // draw vertical line
                        vline(SHIP_CHAR, s.back.y - s.front.y);
                } else {
                        // draw horizontal line
                        hline(SHIP_CHAR, s.back.x - s.front.x);
                }
                getch();
        }
        return 0;
}


int
stop_ncurses(void)
{
        clear();
        curs_set(TRUE);
        echo();
        nocbreak();
        endwin();
        return 0;
}
