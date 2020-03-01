// This file contains functions to test other files
#include "maps.c"
#include "elog.c"
#include "ship.c"

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

void
print_radar(radar r, int num_pieces)
{
	puts("RADAR TEST");

	print_map(r.map);

	// print pieces
	for ( int i = 0; i < num_pieces; i++ )
	{
		printf("piece[%i]: %s, %c, %li\n", i, r.pieces[i].name, (r.pieces[i].sunk ? 'T' : 'F'), r.pieces[i].size);
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
		print_ship(b.pieces[i]);

	printf("\n");
}

void
test_board(void)
{
	ship a = init_ship("Ship A", 5, false, 3, 1);
	ship b = init_ship("Ship B", 4, true, 5, 3);
	ship c = init_ship("Ship C", 3, true, 7, 3);

	ship arr[3] = {a, b, c};
	board B = init_board(arr, 3);

	print_board(B, 3);
}

int
main(void)
{
	test_ship();
	test_radar();
	test_elog();
	test_board();
	return 0;
}
