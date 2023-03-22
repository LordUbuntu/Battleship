#include "network.c"
#include "graphics.c"
#include "data.h"

ship *gen_ships(void);

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


ship* starting_set(void) {
        ship ships[5];
        ship battleship = {FALSE, 5, {0, 0}, {0, 0}, "Battleship"};
        // TODO add more ships
        // to fix address pointer warning, set this variable to be static, or allocate memory in main and then free it later.
        return ships;
}
