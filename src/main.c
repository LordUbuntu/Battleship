#include <string.h>
#include <stdbool.h>
// #include "network.c"  // unstable
#include "graphics.c"
#include "data.h"


int menu(void);
void help(void);
void attack(WINDOW *board, WINDOW *log, pos *position);
void place_ship(WINDOW *board, WINDOW *log, ship *ship);
void place_ships(WINDOW *board, WINDOW *log, ship ships[static NUM_SHIPS]);


int main(void) {
        // INIT GRAPHICS
        init_ncurses();

        // START GAME
        while (true) {
                // INIT DATA
                gamestate state = {
                        .ships = DEFAULT_SHIPS,
                        .pboard = DEFAULT_PINS,
                        .eboard = DEFAULT_PINS,
                        .turn = false,          // initially false for both
                        .winner = UNDECIDED,    // initially UNDECIDED (00) for both
                };
                // MENU
                int selection = menu();
                if (selection == 0) {
                        // setup local variables
                        pos p;
                        // setup game scene
                        WINDOW *player_board = newwin(12, 12, 1, 1);
                        WINDOW *enemy_board = newwin(12, 12, 1, 14);
                        WINDOW *log = newwin(3, 25, 13, 1);
                        box(player_board, 0, 0);
                        box(enemy_board, 0, 0);
                        box(log, 0, 0);
                        wbkgd(player_board, WATER);
                        wbkgd(enemy_board, WATER);
                        wrefresh(player_board);
                        wrefresh(enemy_board);
                        wrefresh(log);
                        refresh();
                        // player place ships
                        place_ships(player_board, log, state.ships);
                        render_ships(player_board, state.ships);
                        // enemy place ships
                        // game loop (taking turns attacking)
                        bool game_running = true;
                        while (game_running) {
                                // player attack
                                attack(enemy_board, log, &p);
                                mvwprintw(log, 1, 1, "Struck Pos: %i,%i", p.x,p.y);
                                wrefresh(log);
                                game_running = false;  // temp
                                // enemy attack
                        }
                        // end single player
                        getch();
                        delwin(player_board);
                        delwin(enemy_board);
                        delwin(log);
                } else if (selection == 1) {
                        // how to save repeating single player code here?
                        // maybe just a change in networking code?
                        // maybe there's just PLAY as an option (0) for what is now "single player" and then this option is just the menu to setup a connection that then puts you into "single player" with the network connecting to another person instead of an AI
                        refresh();
                        printw("Multi Player");
                        refresh();
                        getch();
                } else if (selection == 2) {
                        help();
                } else if (selection == 3) {
                        refresh();
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
}


void place_ships(WINDOW *board, WINDOW *log, ship ships[static NUM_SHIPS]) {
}


void attack(WINDOW *board, WINDOW *log, pos *position) {
        int input = 0;
        char ch = 0;
        int x = 1, y = 1;  // [1, 10] -> [0, 9]

        // get input
        bool valid_input = false;
        while (!valid_input) {
                // highlight current board tile
                move(y, x);
                ch = inch();
                wattron(board, A_REVERSE);
                mvwaddch(board, y, x, mvwinch(board, y, x) & A_CHARTEXT);
                wrefresh(board);

                // get input
                input = getch();
                // clear highlight
                wattroff(board, A_REVERSE);
                mvwaddch(board, y, x, mvwinch(board, y, x) & A_CHARTEXT);
                switch (input) {
                        case KEY_UP:
                                y > 1 ? y-- : y;
                                break;
                        case KEY_DOWN:
                                y < 10 ? y++ : y;
                                break;
                        case KEY_LEFT:
                                x > 1 ? x-- : x;
                                break;
                        case KEY_RIGHT:
                                x < 10 ? x++ : x;
                                break;
                        case '\n':
                                // TODO: verify input on only WATER spaces
                                // verify input
                                // should be ch == WATER, passing for now
                                /*
                                if (ch == ' ') {  // maybe if !in_pins
                                        valid_input = true;
                                } else {
                                        // report invalid input
                                        mvwprintw(log, 1, 1, "Invalid: %i,%i", x, y);
                                        wrefresh(log);
                                }
                                */
                                // update position
                                position->x = --x;
                                position->y = --y;
                                valid_input = true;
                                break;
                        default:
                                break;
                }
                wrefresh(board);
        }
}


// TODO: refactor with option and selection enum for better hackability
int menu(void) {
        clear();
        refresh();
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
        refresh();
        return highlight;
}
