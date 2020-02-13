#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ship.h"

// records the state of each tile, represented either as NONE (~), HIT (*), or MISS (@)
#define NONE 0
#define HIT 1
#define MISS 2
typedef int map[10][10];





/* --------------- BOARD ---------------

   Author:
        Jacobus Burger

   Description:
        A data structure representing the state of this players' map and
        its pieces.

   Specifications:
        - TODO

   Structures/Types:
        Map: the state at each coordinate on the map (NONE, HIT, or MISS)

   Notes:
        - TODO

   -------------------------------------
 */
typedef struct BOARD
{ // records state of player map and pieces
	map map; // coord and state of each tile
	ship* pieces; // knowledge of player pieces
} board;

// creates a board struct with default parameters
board init_board(ship* pieces, int num_pieces);





/* --------------- RADAR ---------------

   Author:
      Jacobus Burger

   Description:
      A data structure representing the known state of opponent map and pieces.

   Specifications:
      - position and state of each tile on the map
      - what enemy pieces are still on the map
        - the name of each ship
        - whether each ship was sunk or not
        - the size of each ship

   Structures/Types:
      Map: the state at each coordinate on the map (NONE, HIT, or MISS)
      ShipData: the name, state, and size of each enemy piece.

   Notes:
      - This entire section took 2 hours over 3 days to come to this version.

   ------------------------------------ -
 */
typedef struct radar
{ // records state of opponent map and pieces
	map map; // coord and state of each tile
	shipdat* pieces; // knowledge of enemy pieces
} radar;

// creates a radar struct with default parameters
radar
init_radar(shipdat* pieces, int num_pieces)
{
	// allocate memory for radar struct
	radar* ra = calloc(sizeof(radar) + sizeof(shipdat) * num_pieces, 1);

	// set temporary radar struct parameters
	ra->pieces = pieces;

	// pass copy to final radar struct
	radar r = *ra;

	// free memory of temporary radar struct
	free(ra);

	// return final initialized structure
	return r;
}
