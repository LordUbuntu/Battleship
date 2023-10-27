#include <string.h>
#include <stdbool.h>
// #include "network.c"  // unstable
#include "graphics.c"
#include "data.h"


// render menu scene and pick a number representing a menu option
int menu(void);
// check if tile
//   has been struck before
//   has a ship on it
bool used_tile(pos tile, map board);
// bool ship_tile(pos tile, ship ships[static NUM_SHIPS]);
bool intersect(pos a1, pos b1, pos a2, pos b2);
// mark a pin on the enemy board if occupied
// TODO: change `pos *position` to `pins *board`
void attack(WINDOW *board, WINDOW *log, pos *position);
// place ships on player board
void place_ship(ship *ship, pos p, bool vertical);
void place_ships(WINDOW *board, WINDOW *log, ship ships[static NUM_SHIPS]);


int main(void) {
        // INIT GRAPHICS
        init_ncurses();

        // START GAME
        while (true) {
                // INIT DATA
                gamestate state = {
                        .ships = DEFAULT_SHIPS,
                        .pboard = DEFAULT_MAP,
                        .eboard = DEFAULT_MAP,
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
                        wbkgd(player_board, WATER);
                        wbkgd(enemy_board, WATER);
                        box(player_board, 0, 0);
                        box(enemy_board, 0, 0);
                        box(log, 0, 0);
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


// preconditions:
//      `tile` is {x,y} at origin {0,0} in range [0,9]
//      `board` is char** map with WATER,MISS,HIT chars
bool used_tile(pos tile, map board) {
        if (board[tile.y][tile.x] == WATER)
                return false;  // if tile is water, it hasn't been used yet
        return true;  // assume used otherwise (MISS/HIT/SHIP/?)
}


// TODO: replace looping logic with intersect function
// preconditions:
//      `tile` is {x,y} at origin {0,0} in range [0,9]
//      `ships` contains vertically/horizontally aligned ship structs
/*
bool ship_tile(pos tile, ship ships[static NUM_SHIPS]) {
        for (int i = 0; i < NUM_SHIPS; i++) {
                ship s = ships[i];
                bool is_ship_tile = intersect(s.front, s.back, tile, tile);
                if (is_ship_tile)
                        return true;
        }
        return false;
}
*/


// determine if two lines intersect
// preconditions:
//      lines are vertical or horizontal only
// this works by checking if the leftmost point of line 2 is bounded in the x of line
// 1 (or if the leftmost point of line 1 is bounded in the x of line 2), then checks
// if the uppermost point of line 2 is bounded in the y of line 1 (or if the
// uppermost point of line 1 is bounded in the y of line 2). This is inclusive since
// we're working with a grid of tiles (pos). This also works if one of the lines is a
// point (the start and end of the line is the same tile on the grid). To see why
// this works or doesn't think about line numbers and draw a few cases on some paper.
bool intersect(pos a1, pos b1, pos a2, pos b2) {
        // IF forward point bounded on x overlapping
        if (a1.x <= a2.x <= b1.x || a2.x <= a1.x <= b2.x)
                // AND forward point bounded on y overlapping
                if (a1.y <= a2.y <= b1.y || a2.y <= a1.y <= b2.y)
                        return true;
        return false;
}


// TODO: add a tooltip window
// TODO: verify ship is not overlapping another one
// TODO: verify ship is not out of bounds for movement and placement
void place_ships(WINDOW *board, WINDOW *log, ship ships[static NUM_SHIPS]) {
        for (int i = 0; i < NUM_SHIPS; i++) {
                int input = 0;
                int x = 1, y = 1;
                ship s = ships[i];
                bool vertical = false;
                bool valid_placement = false;
                while (!valid_placement) {
                        // clear window for fresh redraw
                        wclear(board);
                        box(board, 0, 0);  // wclear erases box border
                        // render previously placed ships
                        for (int j = 0; j < i; j++)
                                render_ship(board, &ships[j]);
                        // render current ship being placed
                        pos front = {x - 1, y - 1};
                        // NOTE: this should be fine since it's a copy
                        place_ship(&s, front, vertical);
                        render_ship(board, &s);
                        // handle input
                        input = getch();
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
                                case 'v':
                                        vertical = !vertical;
                                        break;
                                case '\n':
                                        // verify placement
                                        valid_placement = true;
                                        // place ship
                                        if (valid_placement)
                                                place_ship(&ships[i], front, vertical);
                                        break;
                                default:
                                        break;
                        }
                        // update render
                        wrefresh(board);
                }
        }
}


void place_ship(ship *s, pos front, bool vertical) {
        s->front.x = front.x;
        s->front.y = front.y;
        if (vertical) {
                s->back.x = front.x;
                s->back.y = front.y + s->health;
        } else {
                s->back.y = front.y;
                s->back.x = front.x + s->health;
        }
}


void attack(WINDOW *board, WINDOW *log, pos *position) {
        int input = 0;
        // TODO: replace with `pos cursor`
        int x = 1, y = 1;  // [1, 10] -> [0, 9]

        // get input
        bool valid_input = false;
        while (!valid_input) {
                // highlight current board tile
                move(y, x);
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
                                // TODO: verify input
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
