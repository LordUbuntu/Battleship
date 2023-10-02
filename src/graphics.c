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
                int x, y = s.front.x, s.front.y;
                for(int x, y = s.front.x, s.front.y
                   ;x < s.back.x, y < s.back.y
                   ;x++, y++) {
                        mvaddch(y, x, SHIP_CHAR);
                        getch();  // for debugging
                }
        }
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
