#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "ship.h"

// creates a ship struct with default parameters
// DO NOT FREE positions within this scope, only at end of program
ship
init_ship(char* name, int size, bool vert, int start_x, int start_y)
{
	// allocate memory for temporary ship struct
	ship* temp_ship = calloc(sizeof(ship) + sizeof(pos*), 1);

	// allocate runtime permanent array of positions for said struct
	pos* positions = calloc(sizeof(pos), size);

	// set temporary ship struct parameters
	temp_ship->d.name = name;
	temp_ship->d.sunk = false;
	temp_ship->d.size = size;
	temp_ship->p = positions;

	// set values for array
	for ( int i = 0; i < size; i++ )
	{
		// set temporary pos array parameters
		temp_ship->p[i].x = start_x + (vert ? 0 : i);
		temp_ship->p[i].y = start_y + (vert ? i : 0);
		temp_ship->p[i].hit = false;
	}

	// pass copy to final ship struct
	ship s = *temp_ship;

	// free memory of temporary ship struct
	free(temp_ship);

	// ttt
	// print name and state of ship s
	printf( "%s, %s, %li\n", s.d.name, (s.d.sunk ? "sunk" : "floating"), s.d.size );

	// print coord and state of each pos at index i
	for ( int i = 0; (size_t) i < s.d.size; i++ )
	{
		printf( "(%i, %i, %c)\n", s.p[i].x, s.p[i].y, (s.p[i].hit ? 'T' : 'F') );
	}
	// ttt

	// return final ship struct
	return s;
}

// TODO create destructors to collect everything after the game is done
void
free_ship_posns(ship s)
{
	free(s.p);
}
