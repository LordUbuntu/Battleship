// TODO
// Main file for ncurses display management (and also input for the moment)
#include <ncurses.h>
#include "maps.c"
#include "elog.c"

// render the start menu
void
render_menu(void);

// render the game screen
void
render_game(void);

// print board info (this player's board info)
void
draw_board(board B);

// print radar info (known enemy board info)
void
draw_radar(radar R);

// print elog info
void
draw_elog(elog E);

// echo user input string or err/response string to input bar
void
echo_input(char str[static ENTRY_LENGTH]);
