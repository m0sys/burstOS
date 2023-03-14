CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf main

main: main.o worker.o board.o
	$(CC) $(CFLAGS) -o main main.o worker.o board.o

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

worker.o: src/worker/worker.c
	$(CC) $(CFLAGS) -c src/worker/worker.c

board.o: src/board/board.c
	$(CC) $(CFLAGS) -c src/board/board.c

