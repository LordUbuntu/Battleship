#include <string.h>
#include <stdbool.h>
#include "network.c"
#include "graphics.c"
#include "data.h"

/*
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
*/


// TODO: place piece in x,y location on ship data first, then place as tiles on map
// NOTE: ships are represented through map
/*
int place_ship(map m, ship s, int x, int y, bool vertical) {
        // preconditions:
        // - the ship is at full health (initialized, but game has not started yet)
        
        // PERFORM CHECKS
        // UPDATE SHIP INFO
        // PLACE SHIP ON MAP
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
*/


int menu(void) {
        int input = 0;
        WINDOW *menu = newwin(LINES, COLS, 0, 0);
        keypad(menu, true);
        char option[4][16] = {
                "Single Player",
                "Multi Player",
                "Help",
                "Quit",
        };
        int highlight = 0;  // highlighted menu item
        // menu loop
        bool loop = true;
        while (loop) {
                mvwprintw(menu, 3, 5, "MENU");
                for (int i = 0; i < 4; i++) {
                        if (i == highlight)
                                wattron(menu, A_REVERSE);
                        mvwprintw(menu, 3+(2*(i+1)), 3, "%s", option[i]);
                        wattroff(menu, A_REVERSE);
                }
                mvwprintw(menu, 10, 10, "%i", input);
                wrefresh(menu);
                input = wgetch(menu);
                switch (input) {
                        case KEY_DOWN:
                                highlight = (highlight + 1) % 4;
                                break;
                        case KEY_UP:
                                highlight = (highlight - 1) % 4;
                                highlight = highlight < 0 ? 3 : highlight;
                                break;
                        case '\n':
                                loop = false;
                                break;
                        default:
                                break;
                }
        }
        delwin(menu);
        erase();
        return highlight;
}


// BUG: why is blank on first vision?
void help(void) {
        // this is fine because the help page data is static
        WINDOW *help = newwin(LINES, COLS, 0, 0);
        int input = 0;
        while (true) {
                mvwprintw(help, 1, 1, "The goal of battleship is to sink all");
                mvwprintw(help, 2, 1, "of your opponent's pieces before they");
                mvwprintw(help, 3, 1, "sink yours.");

                mvwprintw(help, 4, 1, "First, place your pieces on your board");
                mvwprintw(help, 5, 1, "using the arrow keys, and ENTER to");
                mvwprintw(help, 6, 1, "confirm your placement. Ensure that");
                mvwprintw(help, 7, 1, "all pieces are not overlapping and");
                mvwprintw(help, 8, 1, "are not out of bounds.");
                mvwprintw(help, 9, 1, "Next, the game starts and you both");
                mvwprintw(help, 10, 1, "take turns using the arrow keys to");
                mvwprintw(help, 11, 1, "select an open spot on the opponent");
                mvwprintw(help, 12, 1, "board, and firing a volley with ENTER.");
                
                mvwprintw(help, 15, 1, "To return to the menu, press 'q'");

                wrefresh(help);
                input = getch();
                if (input == 'q')
                        break;
        }
        delwin(help);
        erase();
}


int main(void) {
        // INITIALIZE GAMESTATE
        // ship player_ships[5] = DEFAULT_SHIPS;
        // map player_map = DEFAULT_MAP;
        // INIT GRAPHICS
        init_ncurses();

        // START GAME
        while (true) {
                int selection = menu();
                if (selection == 0) {
                        printw("Single Player");
                        refresh();
                        getch();
                } else if (selection == 1) {
                        printw("Multi Player");
                        refresh();
                        getch();
                } else if (selection == 2) {
                        help();
                } else if (selection == 3) {
                        printw("Thanks For Playing!");
                        refresh();
                        getch();
                        break;
                } else {
                        printw("CRAP!!!");
                        refresh();
                        getch();
                        break;
                }
        }

        stop_ncurses();


/*
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
*/
}
