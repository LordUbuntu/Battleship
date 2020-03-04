#ifndef SHIP_H
#define SHIP_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct ShipData
{ // records information about the ship
	char* name; // the name of the ship OR what type of it is
	bool sunk; // whether the ship was sunk or not
	size_t size; // how big the ship is
} shipdat;

typedef struct Position
{ // records the coord and state of a given point
	int x, y; // coordinate of point (x and y)
	bool hit; // state of point (hit or miss)
} pos;

typedef struct Ship
{ // records state and position of ships (game pieces)
	shipdat d; // information about the ship
	pos* p; // coordinate and state of each point of the ship
} ship;

ship init_ship(char* name, int size, bool vert, int start_x, int start_y);

#endif /* ifndef SHIP_H */
