/* Jacobus Burger (2023)
 * Abstract data types used throughout the program to represent program
 *   state. 
 */
#pragma once
#include <stdbool.h>


// (x, y) position
typedef struct {
        unsigned x : 4;
        unsigned y : 4;
} pos;


// representation of ship data during game
#define NUM_SHIPS 5
typedef struct {
        unsigned sunk : 1;      // whether the ship is sunk/gone
        unsigned health : 3;    // how many ship tiles aren't hit
        pos front;              // (x,y) pos of ship front
        pos back;               // (x,y) pos of ship back
        const char name[11];    // name of ship type
} ship;
// default ship set
#define DEFAULT_SHIPS {                         \
        {FALSE, 2, {0,0}, {2,0}, "Destroyer"},  \
        {FALSE, 3, {0,1}, {3,1}, "Submarine"},  \
        {FALSE, 3, {0,2}, {3,2}, "Cruiser"},    \
        {FALSE, 4, {0,3}, {4,3}, "Battleship"}, \
        {FALSE, 5, {0,4}, {5,4}, "Carrier"},    \
}


// list of tiles that have been tried ("pinned")
// NOTE: changed to implicitly store the position (2d arr[y][x]) and instead
//       record the state of each tile as WATER ('~'), MISS ('@'), or HIT ('*')
// this way has the benefit of using much less space (char** array vs pos* array)
// and being easier to check (just inch at {x+1,y+1}, or board[y][x])
// and being easier to sync with actual board state (since it basically is)
// AND being easier to render board, by just copying tiles onto window
#define WATER '~'
#define SHIP '#'
#define HIT '*'
#define MISS '@'
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
typedef char map[MAP_HEIGHT][MAP_WIDTH];
#define DEFAULT_MAP { \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}, \
        {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER}  \
}


// winner/loser states
#define UNDECIDED 0b00
#define WINNER 0b01
#define LOSER 0b10


// the notorious gamestate record
typedef struct {
        ship ships[NUM_SHIPS];  // player's ships
        // NOTE: both pins arrays should be syncronized on network
        map pboard;  // enemy's pins in player's board
        map eboard;  // player's pins in enermy's board
        bool turn;    // player's turn, host always starts with this true
        unsigned winner : 2;  // 00 to start, 01 for winner, 10 for loser
} gamestate;
