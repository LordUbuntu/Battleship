// Jacobus Burger
// rationale:
// the reason that both ship and radar functions and structures have
//   pointer based array members and pointer arguments respectively is
//   to allow for ships and ship collections of any given size if desired.
//
// This is because a game may require 7 pieces or 5 pieces. Perhaps a game
// has pieces that are more than 5 tiles long, etc.
//
// This allows some level of future-proofing

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>





// ---------------- SHIP ----------------
//
// Author:
//      Jacobus Burger
//
// Description:
//      A data structure (Ship) that represents the information (ShipData) and
//          location (Positon[]) of this player's pieces.
//
// Specificatons:
//      - The name of the ship
//      - Whether the ship was sunk or not
//      - How big the ship is
//      - Where the ship is (what tiles it covers)
//        - The state of each tile the ship covers (hit or miss)
//
// Structures/Types:
//      Position: the x and y coordinate, as well as the state, of a given point
//      ShipData: the name, state, and size of the ship itself
//      Ship: putting the previous two together
//
// Functions:
//      init_ship: string name, int size, bool vert, int start_x, int start_y
//              Initializes a ship of name `name` and size `size`
//                at the given coord (`start_x`, `start_y`) either
//                vertically or horizontally (`vert`).
//              Notice that `sunk` is false by default, and that `hit` for
//                each position is also false by default.
//      print_ship: ship s
//              Prints information about the ship `s`. Including its
//                name, size, whether its sunk or not, and the tiles it
//                occupies.
//      test_ship: void
//              Creates a dummy ship structure using init_ship and
//                prints its information using print_ship.
//
// Notes:
//      - ShipState was given its own structure instead of being a part of Ship implicitly because
//          it is shared between the Radar structure and the Ship structure.
//      - This entire section took 4 hours over 3 days of thinking and writing to come to this version.
//
// --------------------------------------

typedef struct Position
{ // records the coord and state of a given point
	int x, y; // coordinate of point (x and y)
	bool hit; // state of point (hit or miss)
} pos;

typedef struct ShipData
{ // records information about the ship
	char* name; // the name of the ship OR what type of it is
	bool sunk; // whether the ship was sunk or not
	size_t size; // how big the ship is
} shipdat;

typedef struct Ship
{ // records state and position of ships (game pieces)
	shipdat d; // information about the ship
	pos* p; // coordinate and state of each point of the ship
} ship;

ship
init_ship(char* name, int size, bool vert, int start_x, int start_y)
{
	// create temporary pos array of ship size
	pos temp_positions[size];
	for ( int i = 0; i < size; i++ )
	{
		// set temporary pos array parameters
		temp_positions[i].x = start_x + (vert ? 0 : i);
		temp_positions[i].y = start_y + (vert ? i : 0);
		temp_positions[i].hit = false;
	}

	// allocate memory for temporary ship struct
	ship* temp_ship = calloc(sizeof(ship) + sizeof(pos) * size, 1); // XXX

	// set temporary ship struct parameters
	temp_ship->d.name = name;
	temp_ship->d.sunk = false;
	temp_ship->d.size = size;
	temp_ship->p = temp_positions;

	// pass copy to final ship struct
	ship s = *temp_ship;

	// free memory of temporary ship struct
	free(temp_ship);

	// return final ship struct
	return s;
}

void
print_ship(ship s)
{
	// print name and state of ship s
	printf( "%s, %s, %li\n", s.d.name, (s.d.sunk ? "sunk" : "floating"), s.d.size );

	// print coord and state of each pos at index i
	for ( int i = 0; (size_t) i < s.d.size; i++ )
	{
		printf( "(%i, %i, %c)\n", s.p[i].x, s.p[i].y, (s.p[i].hit ? 'T' : 'F') );
	}
}

void
test_ship(void)
{
	ship s = init_ship("Carrier", 5, true, 3, 1);
	print_ship(s);
}





// --------------- RADAR ---------------
//
// Author:
//      Jacobus Burger
//
// Description:
//      A data structure representing the known state of opponent map and pieces.
//
// Specifications:
//      - position and state of each tile on the map
//      - what enemy pieces are still on the map
//        - the name of each ship
//        - whether each ship was sunk or not
//        - the size of each ship
//
// Structures/Types:
//      Map: the state at each coordinate on the map (NONE, HIT, or MISS)
//      ShipData: the name, state, and size of each enemy piece.
//
// Functions:
//      init_radar: shipdat[] pieces, int size
//              Initialize a radar struct with an all NONE Map and
//                a collection of ships `pieces` of size `size`.
//      print_radar: radar r, int size
//              Print the radar map `Map` and the radar collection of ships `pieces`.
//              Notice that the map is initialized to all 0 by default.
//      test_radar:
//              Creates a dummy ship collection and print the map and ship collection.
//
// Notes:
//      - This entire section took 2 hours over 3 days to come to this version.
//
// -------------------------------------

#define NONE 0
#define HIT 1
#define MISS 2

typedef struct radar
{ // records state of opponent map and pieces
	int Map[10][10]; // coord and state of each tile
	shipdat* pieces; // knowledge of enemy pieces
} radar;

radar // size is the size of the pieces shipdat struct array
init_radar(shipdat* pieces, int size)
{
	// allocate memory for radar struct
	radar* ra = calloc(sizeof(radar) + sizeof(shipdat) * size, 1);

	// set temporary radar struct parameters
	ra->pieces = pieces;

	// pass copy to final radar struct
	radar r = *ra;

	// free memory of temporary radar struct
	free(ra);

	// return final initialized structure
	return r;
}

void // size is the size of the pieces shipdat array in r
print_radar(radar r, int size)
{
	// print map
	printf("\n");
	for ( int y = 0; y < 10; y++ )
	{
		printf("{");
		for ( int x = 0; x < 10; x++ )
		{
			if ( r.Map[y][x] == NONE )
				printf("%c", '~');
			if ( r.Map[y][x] == HIT )
				printf("%c", '*');
			if ( r.Map[y][x] == MISS )
				printf("%c", '@');
		}
		printf("}\n");
	}

	// print pieces
	for ( int i = 0; i < size; i++ )
	{
		printf("piece[%i]: %s, %i, %li\n", i, r.pieces[i].name, r.pieces[i].sunk, r.pieces[i].size);
	}

	printf("\n");
}

void
test_radar(void)
{
	int size = 5;
	shipdat arr[5] = {
		{"A", false, 5},
		{"B", false, 4},
		{"C", true, 3},
		{"D", false, 2},
		{"E", false, 1},
	};

	radar r = init_radar(arr, size);
	print_radar(r, size);
}
