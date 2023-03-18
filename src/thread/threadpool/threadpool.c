/**
 * Implementation of thread pool.
 */

#include "threadpool.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 10
#define NUM_THREADS 3

#define TRUE 1

/* This represents work that has to be completed by a thread in the pool. */
typedef struct {
  void (*function)(void *p);
  void *data;
} task;

/**
 * All the global mutexes/semaphors shared between threads.
 */

/* Keep track of how full the queue is. */
sem_t full_queue;

/* A mutex lock for locking access to queue. */
pthread_mutex_t mutex;

/* The work queue. */
task worktodo[QUEUE_SIZE] = {0};

/* The worker bee. */
pthread_t bees[NUM_THREADS];

/*
 * Insert a task into the queue.
 *
 * Returns 0 if successful or 1 otherwise.
 */
int enqueue(task t) { return 0; }

/* Remove a task from the queue. */
task dequeue() { return worktodo[0]; }

/* The worker thread in the thread pool. */
void *worker(void *param) {
  /* Execute the task only if work is available. */
  execute(worktodo[0].function, worktodo[0].data);

  pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p) { (*somefunction)(p); }

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p) {
  worktodo[0].function = somefunction;
  worktodo[0].data = p;

  return 0;
}

/* Initialize the thread pool. */
void pool_init(void) {
  /* Create all threads. */
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_create(&bees[i], NULL, worker, NULL);

  /* Init semaphore to keep track of the num of tasks in the queue. */
  sem_init(&full_queue, 0, 0); /* share only amoung threads in same proc */

  /* Init the mutex to lock access to queue. */
  pthread_mutex_init(&mutex, NULL);
}

/* Shutdown the thread pool */
void pool_shutdown(void) {
  //
  pthread_join(bees[0], NULL);
}
