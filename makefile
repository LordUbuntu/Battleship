CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
CLIBS=-lncurses
COPTS=-O2

test:
	$(CC) $(CFLAGS) src/test.c -o test

Battleship:
	$(CC) $(CFLAGS) $(COPTS) src/main.c -o Battleship





.PHONY:
clean:
	rm -f test Battleship
