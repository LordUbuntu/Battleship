#include <string.h>
#include <stdbool.h>
// #include "network.c"  // unstable
#include "graphics.c"
#include "data.h"


int menu(void);
void help(void);


void attack(WINDOW *board, WINDOW *log, pos *position) {
        int input = 0;
        char ch = 0;
        int x = 1, y = 1;  // [1, 10] -> [0, 9]

        // get input
        wrefresh(board);
        bool valid_input = false;
        while (!valid_input) {
                // highlight current board tile
                move(y, x);
                ch = inch();
                wattron(board, A_REVERSE);
                mvwaddch(board, y, x, ch);
                wrefresh(board);

                // get input
                input = getch();
                // clear highlight
                wattroff(board, A_REVERSE);
                mvwaddch(board, y, x, ch);
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

void get_pos(WINDOW *win, pos *p) {
        int input = 0;
        char ch = 0;
        int x = 1, y = 1;  // [1, 10] here but return [0, 9]

        wrefresh(win);
        move(y, x);
        ch = inch();
        wattron(win, A_REVERSE);
        mvwaddch(win, y, x, ch);
        wattroff(win, A_REVERSE);
        wrefresh(win);

        bool confirmed = false;
        while (!confirmed) {
                input = getch();
                wattroff(win, A_REVERSE);
                mvwaddch(win, y, x, ch);  // clear previous highlight
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
                                p->x = --x;
                                p->y = --y;
                                confirmed = true;
                                break;
                        default:
                                break;
                }

                if (confirmed) {
                        move(y, x);
                        ch = inch();
                        wattroff(win, A_REVERSE);
                        mvwaddch(win, y, x, ch);
                } else {
                        move(y, x);
                        ch = inch();
                        wattron(win, A_REVERSE);
                        mvwaddch(win, y, x, ch);
                        wattroff(win, A_REVERSE);
                }
                wrefresh(win);
        }

}


int main(void) {
        // INITIALIZE GAMESTATE
        gamestate state = {
                .ships = DEFAULT_SHIPS,
                .board = DEFAULT_MAP,
                .player_pins = {},
                .opponent_pins = {},
                .player_turn = true,
                .turn_number = 0,
                .score = 0,
                .winner = 0
        };
        // ship player_ships[5] = DEFAULT_SHIPS;
        // map player_map = DEFAULT_MAP;
        // INIT GRAPHICS
        init_ncurses();

        // START GAME
        while (true) {
                int selection = menu();
                if (selection == 0) {
                        state.player_turn = true;
                        refresh();
                        // int input = 0;
                        WINDOW *player_board = newwin(12, 12, 1, 1);
                        WINDOW *enemy_board = newwin(12, 12, 1, 14);
                        WINDOW *last_move = newwin(3, 25, 13, 1);
                        box(player_board, 0, 0);
                        box(enemy_board, 0, 0);
                        box(last_move, 0, 0);
                        wrefresh(player_board);
                        wrefresh(enemy_board);
                        wrefresh(last_move);
                        getch();
                        pos p;
                        attack(enemy_board, last_move, &p);
                        // get_pos(enemy_board, &p);
                        mvwprintw(last_move, 1, 1, "Struck Pos: %i,%i", p.x,p.y);
                        wrefresh(last_move);
                        getch();
                } else if (selection == 1) {
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


void help(void) {
        // this is fine because the help page data is static
        clear();
        refresh();
        WINDOW *help = newwin(LINES, COLS, 0, 0);
        int input = 0;
        while (true) {
                if (input == 'q')
                        break;
                mvwprintw(help, 1, 1, "The goal of battleship is to sink all");
                mvwprintw(help, 2, 1, "of your opponent's pieces before they");
                mvwprintw(help, 3, 1, "sink yours.");

                mvwprintw(help, 5, 1, "First, place your pieces on your board");
                mvwprintw(help, 6, 1, "using the arrow keys, and ENTER to");
                mvwprintw(help, 7, 1, "confirm your placement. Ensure that");
                mvwprintw(help, 8, 1, "all pieces are not overlapping and");
                mvwprintw(help, 9, 1, "are not out of bounds.");
                mvwprintw(help, 10, 1, "Next, the game starts and you both");
                mvwprintw(help, 11, 1, "take turns using the arrow keys to");
                mvwprintw(help, 12, 1, "select an open spot on the opponent");
                mvwprintw(help, 13, 1, "board, and firing a volley with ENTER.");
                
                mvwprintw(help, 15, 1, "To return to the menu, press 'q'");

                wrefresh(help);

                input = getch();
        }
        delwin(help);
        erase();
        refresh();
}
