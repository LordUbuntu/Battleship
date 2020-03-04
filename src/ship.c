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

	// allocate runtime-permanent array of positions for said struct
	pos* positions = calloc(sizeof(pos), size);

	// set temporary ship struct parameters
	temp_ship->d.name = name;
	temp_ship->d.sunk = false;
	temp_ship->d.size = size;
	temp_ship->p = positions;

	// set values for array
	for ( int i = 0; i < size; i++ )
	{
		temp_ship->p[i].x = start_x + (vert ? 0 : i);
		temp_ship->p[i].y = start_y + (vert ? i : 0);
		temp_ship->p[i].hit = false;
	}

	// pass copy to final ship struct
	ship s = *temp_ship;

	// free memory of temporary ship struct
	free(temp_ship);

	// return final ship struct
	return s;
}
