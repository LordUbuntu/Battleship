CC=clang
FLAGS=-std=c99 -Wall -Wextra -Werror
LIBS=-lncurses

all:
	$(CC) $(FLAGS) -O3 $(LIBS) src/main.c -o battleship

debug:
	$(CC) $(FLAGS) -Og -g3 -ggdb $(LIBS) src/main.c -o debug

battleship:
	$(CC) $(FLAGS) $(LIBS) src/main.c -o battleship

clean:
	rm debug battleship
