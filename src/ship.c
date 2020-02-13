#include <stdbool.h>
#include <stdlib.h>
#include "ship.h"

// creates a ship struct with default parameters
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
