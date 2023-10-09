#include <string.h>
#include <stdbool.h>
#include "network.c"
#include "graphics.c"
#include "data.h"


int char_to_pos(char msg[static 3], pos *p) {
        int y = msg[0] - 0x41;  // letter part A-J
        int x = msg[1] - 0x30;  // number part 0-9
        // return error if conversion data invalid
        if (y < 0 || y > 9)
                return 1;
        if (x < 0 || x > 9)
                return 1;
        // set coordinates
        p->x = x;
        p->y = y;
        return 0;
}


// TODO: place piece in x,y location on ship data first, then place as tiles on map
int place_ship(map m, ship s, int x, int y, bool vertical) {
        // preconditions:
        // - the ship is at full health (initialized, but game has not started yet)
        if (s.back.x - s.front.x == 0) {
                // fill vertical
                for (int y = s.front.y; y < s.back.y + 1; y++)
                        m[y][s.front.x] = SHIP;
        } else if (s.back.y - s.front.y == 0) {
                // fill horizontal
                for (int x = s.front.x; x < s.back.x + 1; x++)
                        m[s.front.y][x] = SHIP;
        } else {
                return 1;
        }
        return 0;
}


int main(void) {
        // init player data
        ship player_ships[5] = DEFAULT_SHIPS;
        map player_map = DEFAULT_MAP;
        place_ship(player_map, player_ships[0], 0, 1, true);


        // render graphics
        init_ncurses();
        // render empty player board
        //                             w,  h, y, x
        WINDOW *player_board = newwin(12, 12, 4, 2);
        wborder(player_board, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(player_board);
        // render empty input box
        WINDOW *text_box = newwin(3, COLS, LINES - 3, 0);
        wborder(text_box, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(text_box);

        // get input box input
        char msg[3];
        wgetstr(text_box, msg);
        wmove(text_box, 1, 1);
        wprintw(text_box, "Strike: %.2s", msg);
        wrefresh(text_box);

        // show actual coordinates
        wmove(text_box, 1, 1);
        pos p = { 1, 1 };
        int status = char_to_pos(msg, &p);
        if (status) {
                wprintw(text_box, "conversion failed! %i", status);
        } else {
                wprintw(text_box, "Coordinate is: %iy %ix", p.y, p.x);
        }
        wrefresh(text_box);

        wgetch(text_box);

        // render map data
        render_map(player_board, player_map);
        wgetch(player_board);
        // render ships
        render_ships(player_board, player_ships);
        wgetch(player_board);


        // render main screen
        //   if user selects single player
        //     render game screen
        //     start bot
        //     return to main screen when game is done
        //   if player selects network player
        //     render connection screen
        //     if player successfully connects to another player
        //       render game screen
        //       return to main screen when game is done
        //   if player selects help, render help screen
        //   if player selects quit, end program
        // stop curses graphics
        stop_ncurses();
}
