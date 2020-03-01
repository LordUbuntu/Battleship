// This file contains functions to test the functions and structs of other files in the project
#include "maps.c"
#include "elog.c"
#include "ship.c"

void
print_radar(radar r, int num_pieces)
{
	printf("RADAR TEST: \n");

	print_map(r.map);

	// print pieces
	for ( int i = 0; i < num_pieces; i++ )
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

void
print_elog(elog e)
{
	printf("EVENT LOG TEST: \n");

	for ( int i = 0; i < ENTRY_NUM; i++ )
		printf("[%i] %s\n", i, e.evlog[i]);
	printf("\n");
}

void
test_elog(void)
{
	elog e = init_elog();
	e.index = ENTRY_NUM - 2;
	print_elog(e);
	for ( int i = 0; i < 3; i++ )
		add_elog_entry(&e, "New Entry!");
	print_elog(e);
}

void
print_board(board b, int num_pieces)
{
	printf("BOARD TEST:\n");

	print_map(b.map);

	// print pieces
	for ( int i = 0; i < num_pieces; i++ )
	{
		printf("piece[%i]: %s, %i, %li\n", i, b.pieces[i].d.name, b.pieces[i].d.sunk, b.pieces[i].d.size);
		for ( int i = 0; (size_t) i < b.pieces[i].d.size; i++ )
		{
			// XXX for some reason this displays all position data as 0 with all
			//      position arrays being the same size
			printf( "(%i, %i, %c)\n", b.pieces[i].p[i].x, b.pieces[i].p[i].y, (b.pieces[i].p[i].hit ? 'T' : 'F') );
		}
	}

	printf("\n");
}

// TODO complete and test
void
test_board(void)
{
	ship piece1 = init_ship("Ship A", 5, false, 3, 1);
	ship piece2 = init_ship("Ship B", 4, false, 5, 1);
	ship piece3 = init_ship("Ship C", 3, false, 7, 1);
	ship piece_arr[3] = { piece1, piece2, piece3 };
	board b = init_board(piece_arr, 3);
	print_board(b, 3);
}

void
test_ship(void)
{
	puts("test ship create ship s");
	ship s = init_ship("Carrier", 5, true, 3, 1);
	puts("test ship print ship s");
	print_ship(s);
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

int
main(void)
{
	puts("create ship s");
	ship s = init_ship("Ship A", 5, true, 2, 1);    // XXX pos is not garbage here

	puts("print ship s");
	print_ship(s);                                  // but it is here

	puts("run test_ship");
	test_ship();                                    // and here
	return 0;
}
