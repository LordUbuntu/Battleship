CC=clang
FLAGS=-std=c99 -Wall -Wextra
LIBS=-lncurses


debug:
	$(CC) $(FLAGS) -Og -g3 -ggdb $(LIBS) src/main.c -o debug

battleship:
	$(CC) $(FLAGS) $(LIBS) src/main.c -o battleship

clean:
	rm debug battleship
