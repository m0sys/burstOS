CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf main

main: main.o sort.o worker.o arr.o
	$(CC) $(CFLAGS) -o main main.o sort.o worker.o arr.o

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

sort.o: src/sort/sort.c
	$(CC) $(CFLAGS) -c src/sort/sort.c

worker.o: src/worker/worker.c
	$(CC) $(CFLAGS) -c src/worker/worker.c

arr.o: src/util/arr.c
	$(CC) $(CFLAGS) -c src/util/arr.c
