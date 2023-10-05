#include <string.h>
#include "network.c"
#include "graphics.c"
#include "data.h"


// NOTE:
// A principle of design in this program is the division between data and fuction, and what is rendered. What is rendered is based on the current state of data, that data is changed by the opertion of procedures. This loosely follows the update and draw loop design common in a lot of games.


void init_ships(ship *ships_arr) {
        static ship ships[NUM_SHIPS] = {
                {FALSE, 2, {0,0}, {2,0}, "Destroyer"},
                {FALSE, 3, {0,1}, {3,1}, "Submarine"},
                {FALSE, 3, {0,2}, {3,2}, "Cruiser"},
                {FALSE, 4, {0,3}, {4,3}, "Battleship"},
                {FALSE, 5, {0,4}, {5,4}, "Carrier"},
        };
        memcpy(ships_arr, ships, sizeof(ship));
}


int main(void) {
        // init curses graphics
        init_ncurses();


        // render empty player board
        //                             w,  h, y, x
        WINDOW *player_board = newwin(12, 12, 4, 2);
        wborder(player_board, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(player_board);

        // render empty input box
        WINDOW *text_box = newwin(3, COLS, LINES - 3, 0);
        wborder(text_box, '|', '|', '-', '-', '+', '+', '+', '+');
        wmove(text_box, 1, 1);
        wprintw(text_box, "Coordinate...");
        wrefresh(text_box);

        // get input box input
        char msg[3];
        wgetstr(text_box, msg);
        msg[2] = '\n';
        wmove(text_box, 1, 1);
        wprintw(text_box, "Strike: %.2s", msg);
        wrefresh(text_box);

        // show actual coordinates
        wmove(text_box, 1, 1);
        wprintw(text_box, "Coordinate is: %iy %ix", (msg[0] - 0x41 + 1), (msg[1] - 0x30));
        wrefresh(text_box);

        // initialize and render ships
        ship ships[NUM_SHIPS];
        init_ships(ships);
        render_ships(ships, player_board);

        wgetch(text_box);


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
