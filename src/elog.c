/* --------------- ELOG ---------------

   Author:
        Jacobus Burger

   Description:
        An array of strings representing sequential entries in an event log, and
        an index representing the current position in the circular que of entries.

   Specifications:
        - An index representing the position in the queue
        - An array of strings representing each logged event

   Notes:
        - Events are not logged or recorded concretely on their own, there are
            some reasons why it might be prefereable to do it that way, but I
            have found it more efficient to record state with maps and ships keeping
            only the relevant parts, and consequently only representing events as an
            abstract representation of existing data.

   ------------------------------------
 */
#include <string.h>

#define ENTRY_NUM 10 // the number of entries the elog contains
#define ENTRY_LENGTH 20 // the number of characters each entry can contain
#define ENTRY_DEFAULT " .                 " // the default entry

typedef struct ELOG
{ // records log of events
	int index; // current index being used
	char evlog[ENTRY_NUM][ENTRY_LENGTH]; // the log itself
} elog;

// creates an elog struct with default parameters
elog
init_elog(void)
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

// takes a pointer to an elog structure and a string and adds that string to the log at the current index
void
add_elog_entry(elog* e, char entry[static ENTRY_LENGTH])
{
	// write entry to log at index
	memcpy(e->evlog[e->index], entry, sizeof(char) * ENTRY_LENGTH);

	// increment index
	e->index = (e->index + 1) % ENTRY_NUM;

	// prepare current index
	//memcpy(e->evlog[e->index], ENTRY_DEFAULT, sizeof(char) * ENTRY_LENGTH);
}
