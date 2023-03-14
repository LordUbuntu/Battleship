/* Jacobus Burger (2023)
 * Abstract data types used throughout the program to represent program
 *   state. 
 * This is just a draft of expected ADTs for now...
 */


// NOTE: the visual representation of the board is updated one
//      char at a time, but its visualization need not be
//      represented by a board state (at least not at the moment).


// (x, y) position
typedef struct POS {
        unsigned x : 4;
        unsigned y : 4;
} pos;


// representation of ship data during game
typedef struct SHIP {
        unsigned sunk : 1;      // whether the ship is sunk/gone
        unsigned health : 3;    // how many ship tiles aren't hit
        pos front;              // (x,y) pos of ship front
        pos back;               // (x,y) pos of ship back
        char name[10];          // name of ship type
} ship;


// 10x10 player and opponent board
#define WATER 0
#define SHIP 1
typedef char map[10][10];       


// list of tiles that have been tried ("pinned")
typedef pos pins[100];
