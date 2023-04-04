# makefile for sleeping ta problem.

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

clean:
	rm -rf *.o
	rm -rf main

main: main.o 
	$(CC) $(CFLAGS) -o main main.o  $(PHTHREADS)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

