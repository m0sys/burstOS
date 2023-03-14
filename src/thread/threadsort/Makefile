CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf main

main: main.o 
	$(CC) $(CFLAGS) -o main main.o 

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c
