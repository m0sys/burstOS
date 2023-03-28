# makefile for vanila producer/consumer problem.
#
# $SLEEP - how long for main to sleep before termination.
# $NP    - number of producer threads.
# $NC    - number of consumer threads.

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: main.o buffer.o worker.o
	$(CC) $(CFLAGS) -o suey main.o buffer.o worker.o $(PTHREADS) $(SLEEP) $(NP) $(NC)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(PTHREADS)

buffer.o: buffer/buffer.c buffer/buffer.h
	$(CC) $(CFLAGS) -c buffer/buffer.c $(PTHREADS)

worker.o: worker/worker.c worker/worker.h
	$(CC) $(CFLAGS) -c worker/worker.c $(PTHREADS)

clean:
	rm -rf *.o
	rm -rf suey

