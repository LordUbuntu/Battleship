// The event log (or elog) records this player messages and other player responses
#include <stdio.h>
#include <string.h>

#define ENTRY_NUM 10
#define ENTRY_LENGTH 20
#define ENTRY_DEFAULT "`.`````````````````"

typedef struct ELOG
{ // records current
	int index; // current index being used
	char evlog[ENTRY_NUM][ENTRY_LENGTH]; // the log
} elog;

elog
init_log(void)
{
	// create 0'd elog struct
	elog e = {
		.index = 0,
		.evlog = {},
	};

	// copy default to e.evlog
	char default_str[ENTRY_LENGTH] = ENTRY_DEFAULT;
	for ( int i = 0; i < ENTRY_NUM; i++ )
		memcpy(e.evlog[i], default_str, sizeof(char) * ENTRY_LENGTH);

	// return the initialized struct
	return e;
}

void
print_log(elog e)
{
	for ( int i = 0; i < ENTRY_NUM; i++ )
		printf("[%i] %s\n", i, e.evlog[i]);
}

void
test_log(void)
{
	elog e = init_log();
	print_log(e);
}

// TODO more functions to consider?
