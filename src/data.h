/* Jacobus Burger (2023)
 * Abstract data types used throughout the program to represent program
 *   state. 
 * This is just a draft of expected ADTs for now...
 */
#pragma once


// NOTE: the visual representation of the board is updated one
//      char at a time, but its visualization need not be
//      represented by a board state (at least not at the moment).


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


// 10x10 player and opponent board
#define WATER 0
#define SHIP 1
typedef char map[10][10];


// list of tiles that have been tried ("pinned")
typedef pos pins[100];
