#include "network.c"
#include "graphics.c"
#include "data.h"


ship make_ship(char *name, size_t size, unsigned fx, unsigned fy, unsigned bx, unsigned by) {
        // preconditions
        // - size less than MAX_SHIP_LEN
        // - name less than or equal to 10
        // - front and back pos are aligned along dX or dY, not diagonal
        ship s = {0, size, {fx, fy}, {bx, by}, *name};
        return s;
}


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
        ship s = make_ship("Battleship", 5, 0, 2, 0, 3);
}
