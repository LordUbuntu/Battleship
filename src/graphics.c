#include <ncurses.h>


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
stop_ncurses(void)
{
        curs_set(TRUE);
        echo();
        nocbreak();
        endwin();
        return 0;
}
