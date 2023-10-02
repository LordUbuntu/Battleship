#include <string.h>
#include "network.c"
#include "graphics.c"
#include "data.h"


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
        // NOTE: temp example
        ship ships[NUM_SHIPS];
        init_ships(ships);
        render_ships(ships);
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
