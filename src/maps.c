#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ship.h"


// ----- MAP ------
#define NONE 0
#define HIT 1
#define MISS 2
typedef int map[10][10];


// ----- BOARD -----
typedef struct BOARD
{ // records state of player map and pieces
	map map; // coord and state of each tile
	ship* pieces; // knowledge of player pieces
} board;

board
init_board(ship* pieces, int num_pieces)
{
	// allocate memory for temporary board struct
	board* bo = calloc(sizeof(board) + sizeof(pieces) * num_pieces, 1);

	// set temporary board parameters
	bo->pieces = pieces;

	// pass copy of temp board to final board struct
	board b = *bo;

	// free temporary board struct
	free(bo);

	// return final board struct
	return b;
}


// ----- RADAR -----
typedef struct RADAR
{ // records state of opponent map and pieces
	map map; // coord and state of each tile
	shipdat* pieces; // knowledge of enemy pieces
} radar;

radar
init_radar(shipdat* pieces, int num_pieces)
{
	// allocate memory for temp radar struct
	radar* ra = calloc(sizeof(radar) + sizeof(shipdat) * num_pieces, 1);

	// set temporary radar struct parameters
	ra->pieces = pieces;

	// pass copy to final radar struct
	radar r = *ra;

	// free memory of temporary radar struct
	free(ra);

	// return final radar struct
	return r;
}
