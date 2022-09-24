CC=clang
FLAGS=-std=c99 -Wall -Wextra
LIBS=-lncurses


debug:
	$(CC) $(FLAGS) -Og -ggdb $(LIBS) src/main.c -o debug

battleship:
	$(CC) $(FLAGS) $(LIBS) src/main.c -o battleship


debug-nocurses:
	$(CC) $(FLAGS) -Og -ggdb src/main.c -o debug-noc

battleship-nocurses:
	$(CC) $(FLAGS) $(LIBS) src/main.c -o battleship-noc


.PHONY:
	clean:
		rm debug battleship debug-noc battleship-noc
