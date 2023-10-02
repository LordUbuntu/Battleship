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
                int x = s.front.x, y = s.front.y;
                while (x <= s.back.x && y <= s.back.y) {
                        int status = mvaddch(y++, x++, SHIP_CHAR);
                        if (status) return status;
                        getch();  // for debugging
                }
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
