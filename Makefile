CC=gcc


all: main
main: utils.o
utils.o: main.c utils.h utils.h

clean:
		rm -f main utils.o
run: main
		./main
