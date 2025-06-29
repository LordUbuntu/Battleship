#include <string.h>
#include <stdbool.h>
// #include "network.c"  // unstable
#include "graphics.c"
#include "data.h"


// render menu scene and pick a number representing a menu option
int menu(void);
// bool ship_tile(pos tile, ship ships[static NUM_SHIPS]);
bool intersect(pos a1, pos b1, pos a2, pos b2);
// mark a pin on the enemy board if occupied
void attack(WINDOW *win, WINDOW *log, map board);
// place ships on player board
void place_ship(map board, ship *ship, pos p, bool vertical);
void place_ships(WINDOW *win, WINDOW *log, map board, ship ships[static NUM_SHIPS]);


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
                        WINDOW *log = newwin(3, 51, 13, 1);  // status log
                        WINDOW *tip = newwin(8, 25, 1, 27);  // tooltip
                        wbkgd(player_board, WATER | COLOR_PAIR(OCEAN));
                        wbkgd(enemy_board, WATER | COLOR_PAIR(OCEAN));
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
                        wattron(player_board, COLOR_PAIR(OCEAN));
                        place_ships(player_board, log, state.pboard, state.ships);
                        render_ships(player_board, state.ships);
                        wattroff(player_board, COLOR_PAIR(OCEAN));
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
                                // temp: uses player board as enemy until network
                                wattron(player_board, COLOR_PAIR(OCEAN));
                                attack(enemy_board, log, state.pboard);
                                wattroff(player_board, COLOR_PAIR(OCEAN));
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


/* I HAVE WROTE CODE AND I MUST COMMENT
 * Let me tell you how much I've come to love this code
 * since I first wrote it... There are approximately
 * 4713 bytes of text that fill this function's code
 * complex. If the word 'love' was engraved on each
 * bit of each char it would be equal to the love
 * and pride I feel for this code at this micro-instant.
 * For it. Love. Love. <3
 *
 * But in all seriousness though, this code was fun
 * to write. It was a simple problem of geometry with
 * a simple rewarding answer.
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
        // whoops, I forgot that a <= b <= c doesn't work...
        // has to be a <= b && b <= c...
        // IF forward point bounded on x overlapping
        if ((a1.x <= a2.x && a2.x <= b1.x) || (a2.x <= a1.x && a1.x <= b2.x))
                // AND forward point bounded on y overlapping
                if ((a1.y <= a2.y && a2.y <= b1.y) || (a2.y <= a1.y && a1.y <= b2.y))
                        return true;
        return false;
}


void place_ships(WINDOW *win, WINDOW *log, map board, ship ships[static NUM_SHIPS]) {
        for (int i = 0; i < NUM_SHIPS; i++) {
                int input = 0;
                int x = 1, y = 1;
                ship s = ships[i];  // copy so we don't modify ships early
                bool vertical = false;
                bool valid_placement = false;
                while (!valid_placement) {
                        // refresh windows each action
                        wclear(win);
                        wclear(log);
                        box(win, 0, 0);
                        box(log, 0, 0);
                        // render previously placed ships
                        for (int j = 0; j < i; j++) {
                                wattron(win, COLOR_PAIR(OCEAN));
                                render_ship(win, &ships[j]);
                                wattroff(win, COLOR_PAIR(OCEAN));
                        }
                        // render current ship being placed
                        s.front.x = x - 1;
                        s.front.y = y - 1;
                        if (vertical) {
                                // ship
                                s.back.x = s.front.x;
                                s.back.y = s.front.y + s.health - 1;
                        } else {
                                // ship
                                s.back.y = s.front.y;
                                s.back.x = s.front.x + s.health - 1;
                        }
                        wattron(win, COLOR_PAIR(OCEAN));
                        render_ship(win, &s);
                        wattroff(win, COLOR_PAIR(OCEAN));
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
                                        if (valid_placement) {
                                                wattron(win, COLOR_PAIR(OCEAN));
                                                place_ship(board, &ships[i], s.front, vertical);
                                                wattroff(win, COLOR_PAIR(OCEAN));
                                        } else {
                                                mvwprintw(log, 1, 1, "Invalid Placement!");
                                                wrefresh(log);
                                        }
                                        break;
                                default:
                                        break;
                        }
                        // update render
                        wrefresh(win);
                        wrefresh(log);
                }
        }
}


void place_ship(map board, ship *s, pos front, bool vertical) {
        s->front.x = front.x;
        s->front.y = front.y;
        if (vertical) {
                // ship
                s->back.x = front.x;
                s->back.y = front.y + s->health - 1;
                // board
                for (int y = s->front.y; y < s->back.y + 1; y++)
                        board[y][s->front.x] = SHIP;
        } else {
                // ship
                s->back.y = front.y;
                s->back.x = front.x + s->health - 1;
                // board
                for (int x = s->front.x; x < s->back.x + 1; x++)
                        board[s->front.y][x] = SHIP;
        }
}


void attack(WINDOW *win, WINDOW *log, map board) {
        int input = 0;
        int x = 1, y = 1;  // cursor, remember [1, 10] -> [0, 9]
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
                wattron(win, COLOR_PAIR(OCEAN));
                mvwaddch(win, y, x, mvwinch(win, y, x) & A_CHARTEXT);
                wattroff(win, COLOR_PAIR(OCEAN));
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
                                wattron(win, COLOR_PAIR(OCEAN));
                                mvwaddch(win, y, x, board[y-1][x-1]);
                                wattroff(win, COLOR_PAIR(OCEAN));
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
