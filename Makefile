CC=gcc


all: tp1
tp1: utils.o
utils.o: tp1.c utils.h utils.h

clean:
		rm -f tp1 utils.o
run: tp1
		./tp1
