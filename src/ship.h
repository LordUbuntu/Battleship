/* ---------------- SHIP ----------------

   Author:
      Jacobus Burger

   Description:
      A data structure (Ship) that represents the information (ShipData) and
          location (Positon[]) of this player's pieces.

   Specificatons:
      - The name of the ship
      - Whether the ship was sunk or not
      - How big the ship is
      - Where the ship is (what tiles it covers)
        - The state of each tile the ship covers (hit or miss)

   Structures/Types:
      Position: the x and y coordinate, as well as the state, of a given point
      ShipData: the name, state, and size of the ship itself
      Ship: putting the previous two together

   Notes:
      - ShipState was given its own structure instead of being a part of Ship implicitly because
          it is shared between the Radar structure and the Ship structure.
      - This player's map can be represented through a collection of their pieces.
      - This entire section took 4 hours over 3 days of thinking and writing to come to this version.

   --------------------------------------
 */
#ifndef SHIP_H
#define SHIP_H
#include <stdlib.h>

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

// creates a ship struct with default parameters
ship init_ship(char* name, int size, bool vert, int start_x, int start_y);
void print_ship(ship s);

#endif /* ifndef SHIP_H */
