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
bool used_tile(int x, int y, map board);
// bool ship_tile(pos tile, ship ships[static NUM_SHIPS]);
bool intersect(pos a1, pos b1, pos a2, pos b2);
// mark a pin on the enemy board if occupied
void attack(WINDOW *win, WINDOW *log, map board);
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
                        // setup game scene
                        WINDOW *player_board = newwin(12, 12, 1, 1);
                        WINDOW *enemy_board = newwin(12, 12, 1, 14);
                        WINDOW *log = newwin(3, 25, 13, 1);  // status log
                        WINDOW *tip = newwin(8, 25, 1, 27);  // tooltip
                        wbkgd(player_board, WATER);
                        wbkgd(enemy_board, WATER);
                        box(player_board, 0, 0);
                        box(enemy_board, 0, 0);
                        box(log, 0, 0);
                        box(tip, 0, 0);
                        wrefresh(player_board);
                        wrefresh(enemy_board);
                        wrefresh(log);
                        wrefresh(tip);
                        refresh();
                        // update tooltip for ship placement mode
                        mvwprintw(tip, 1, 1, "ENTER - place ship");
                        mvwprintw(tip, 2, 1, "V     - rotate ship");
                        mvwprintw(tip, 3, 1, "UP    - move ship up");
                        mvwprintw(tip, 4, 1, "DOWN  - move ship down");
                        mvwprintw(tip, 5, 1, "LEFT  - move ship left");
                        mvwprintw(tip, 6, 1, "RIGHT - move ship right");
                        wrefresh(tip);
                        // player place ships
                        place_ships(player_board, log, state.ships);
                        render_ships(player_board, state.ships);
                        // enemy place ships
                        // game loop (taking turns attacking)
                        bool game_running = true;
                        while (game_running) {
                                // update tooltip for main game loop
                                werase(tip);
                                box(tip, 0, 0);
                                mvwprintw(tip, 1, 1, "ENTER - attack tile");
                                mvwprintw(tip, 2, 1, "UP    - move up");
                                mvwprintw(tip, 3, 1, "DOWN  - move down");
                                mvwprintw(tip, 4, 1, "LEFT  - move left");
                                mvwprintw(tip, 5, 1, "RIGHT - move right");
                                wrefresh(tip);
                                // player attack
                                attack(player_board, log, state.pboard);
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
        // whoops, I forgot that a <= b <= c doesn't work...
        // has to be a <= b && b <= c...
        // IF forward point bounded on x overlapping
        if ((a1.x <= a2.x && a2.x <= b1.x) || (a2.x <= a1.x && a1.x <= b2.x))
                // AND forward point bounded on y overlapping
                if ((a1.y <= a2.y && a2.y <= b1.y) || (a2.y <= a1.y && a1.y <= b2.y))
                        return true;
        return false;
}


// TODO: add a tooltip window
void place_ships(WINDOW *board, WINDOW *log, ship ships[static NUM_SHIPS]) {
        for (int i = 0; i < NUM_SHIPS; i++) {
                int input = 0;
                int x = 1, y = 1;
                ship s = ships[i];  // copy so we don't modify ships early
                bool vertical = false;
                bool valid_placement = false;
                while (!valid_placement) {
                        // refresh windows each action
                        wclear(board);
                        wclear(log);
                        box(board, 0, 0);
                        box(log, 0, 0);
                        // render previously placed ships
                        for (int j = 0; j < i; j++)
                                render_ship(board, &ships[j]);
                        // render current ship being placed
                        pos front = {x - 1, y - 1};
                        place_ship(&s, front, vertical);
                        render_ship(board, &s);
                        // handle input
                        input = getch();
                        switch (input) {
                                case KEY_UP:
                                        // bound cursor to keep front of ship inside map
                                        y > 1 ? y-- : y;
                                        break;
                                case KEY_DOWN:
                                        // bound cursor to keep back of ship inside map
                                        if (vertical)
                                                y + s.health - 1 < 10 ? y++ : y;
                                        else
                                                y < 10 ? y++ : y;
                                        break;
                                case KEY_LEFT:
                                        // bound cursor to keep front of ship inside map
                                        x > 1 ? x-- : x;
                                        break;
                                case KEY_RIGHT:
                                        // bound cursor to keep back of ship inside map
                                        if (!vertical)
                                                x + s.health - 1 < 10 ? x++ : x;
                                        else
                                                x < 10 ? x++ : x;
                                        break;
                                case 'v':
                                        // correct placement of ship so rotation stays in bounds of map
                                        if (vertical && x + s.health - 1 >= 10)
                                                x = 10 - (s.health - 1);
                                        if (!vertical && y + s.health - 1 >= 10)
                                                y = 10 - (s.health - 1);
                                        // and rotate ship
                                        vertical = !vertical;
                                        break;
                                case '\n':
                                        // verify placement of ship
                                        valid_placement = true;
                                        for (int j = 0; j < i; j++)
                                                if (intersect(s.front, s.back, ships[j].front, ships[j].back))
                                                    valid_placement = false;
                                        // and place ship
                                        if (valid_placement)
                                                place_ship(&ships[i], front, vertical);
                                        else {
                                                mvwprintw(log, 1, 1, "Invalid Placement!");
                                                wrefresh(log);
                                        }
                                        break;
                                default:
                                        break;
                        }
                        // update render
                        wrefresh(board);
                        wrefresh(log);
                }
        }
}


void place_ship(ship *s, pos front, bool vertical) {
        s->front.x = front.x;
        s->front.y = front.y;
        if (vertical) {
                s->back.x = front.x;
                s->back.y = front.y + s->health - 1;
        } else {
                s->back.y = front.y;
                s->back.x = front.x + s->health - 1;
        }
}


// preconditions:
//      `tile` is {x,y} at origin {0,0} in range [0,9]
//      `board` is char** map with WATER,MISS,HIT chars
// WARN: FAULTY LOGIC, doesn't yet match visual state of board
// TODO: implement render for board
// TODO: sync visual and data boards
bool used_tile(int x, int y, map board) {
        if (board[y][x] == WATER)
                return false;  // if tile is water, it hasn't been used yet
        return true;  // assume used otherwise (MISS/HIT/SHIP/?)
}


void attack(WINDOW *win, WINDOW *log, map board) {
        int input = 0;
        // TODO: replace with `pos cursor`
        pos p = {1, 1};  // remember to translate [1, 10] -> [0, 9]
        int x = 1, y = 1;  // [1, 10] -> [0, 9]
        char tile = WATER;

        // get input
        bool valid_input = false;
        while (!valid_input) {
                // highlight current window tile
                move(y, x);
                wattron(win, A_REVERSE);
                mvwaddch(win, y, x, mvwinch(win, y, x) & A_CHARTEXT);
                wrefresh(win);
                werase(log);
                box(log, 0, 0);

                // get input
                input = getch();
                // clear highlight
                wattroff(win, A_REVERSE);
                mvwaddch(win, y, x, mvwinch(win, y, x) & A_CHARTEXT);
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
                        // WARN: placeholder until networking
                        case '\n':
                                // verify position on board
                                tile = board[y-1][x-1]; // note: this will be replaced by a network call later
                                if (tile == SHIP || tile == WATER) {
                                        valid_input = true;
                                        mvwprintw(log, 1, 1, "HIT: %c %i,%i", tile, y-1, x-1);
                                } else {
                                        mvwprintw(log, 1, 1, "INVALID TILE");
                                }
                                // update board tiles and window character
                                if (tile == SHIP) {
                                        board[y-1][x-1] = HIT;
                                }
                                if (tile == WATER) {
                                        board[y-1][x-1] = MISS;
                                }
                                mvwaddch(win, y, x, board[y-1][x-1]);
                                break;
                        default:
                                break;
                }
                wrefresh(win);
                wrefresh(log);
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
