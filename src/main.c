#include "network.c"
#include "graphics.c"
#include "data.h"
#include <assert.h>
#include <string.h>


ship make_ship(char *name, size_t size, pos front, pos back) {
        // preconditions
        // - size less than MAX_SHIP_LEN
        // - name less than or equal to 10
        // - front and back pos are aligned along dX or dY, not diagonal
        assert (size <= MAX_SHIP_LEN);
        assert (strlen(name) <= MAX_SHIP_NAME_LEN);
        assert ((front.x == back.x && front.y != back.y) || (front.x != back.x && front.y == back.y));
        ship s = {0, size, front, back, *name};
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
}
