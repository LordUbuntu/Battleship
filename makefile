CC=clang
FLAGS=-std=c99 -Wall -Wextra
LIBS=-lncurses


debug:
	$(CC) $(FLAGS) -Og -ggdb $(LIBS) src/main.c -o debug

battleship:
	$(CC) $(FLAGS) $(LIBS) src/main.c -o battleship

.PHONY:
	clean:
		rm debug battleship debug-noc battleship-noc
