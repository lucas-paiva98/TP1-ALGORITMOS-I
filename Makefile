all: teste

teste: teste.o main.o 
	gcc -o teste teste.o main.o

teste.o: teste.c
	gcc -o teste.o -c teste.c -W -Wall -ansi -pedantic

main.o: main.c teste.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic

clean:
	rm -rf *.o
