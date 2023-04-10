#include "network.c"
#include "graphics.c"
#include "data.h"

int main(void) {
        // init curses graphics
        init_ncurses();
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


ship *init_ships() {
        static ship ships[5] = {
                {FALSE, 2, {0,0}, {0,0}, "Destroyer"},
                {FALSE, 3, {0,0}, {0,0}, "Submarine"},
                {FALSE, 3, {0,0}, {0,0}, "Cruiser"},
                {FALSE, 4, {0,0}, {0,0}, "Battleship"},
                {FALSE, 5, {0,0}, {0,0}, "Carrier"},
        };
        return ships;
}
