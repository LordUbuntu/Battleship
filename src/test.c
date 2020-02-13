// This file contains functions to test the functions and structs of other files in the project
#include "maps.c"
#include "elog.c"
#include "ship.c"

void
print_elog(elog e)
{
	for ( int i = 0; i < ENTRY_NUM; i++ )
		printf("[%i] %s\n", i, e.evlog[i]);
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
	// print map
	printf("\n");
	for ( int y = 0; y < 10; y++ )
	{
		printf("{");
		for ( int x = 0; x < 10; x++ )
		{
			if ( r.map[y][x] == NONE )
				printf("%c", '~');
			if ( r.map[y][x] == HIT )
				printf("%c", '*');
			if ( r.map[y][x] == MISS )
				printf("%c", '@');
		}
		printf("}\n");
	}

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

int
main(void)
{
	test_ship();
	test_radar();
	test_elog();
	return 0;
}
