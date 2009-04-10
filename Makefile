# basic makefile

all: cpong

cpong: main.o score.o
	gcc -Wall -o cpong main.o score.o -lcurses

main.o: main.c
	gcc -Wall -c -o main.o main.c

score.o: score.c
	gcc -Wall -c -o score.o score.c

clean:
	rm score.o main.o

