/**
 * Implementation of thread pool.
 */

#include "threadpool.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/semaphore.h>

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

/* Next queue insert pos. */
int nxt_pos = 0;

/*
 * Insert a task into the queue.
 *
 * Returns 0 if successful or 1 otherwise.
 */
int enqueue(task t) {
  if (nxt_pos == QUEUE_SIZE)
    return 1;

  worktodo[nxt_pos].function = t.function;
  worktodo[nxt_pos].data = t.data;
  nxt_pos += 1;

  return 0;
}

/* Remove a task from the queue. */
task dequeue() {
  /* FIFO. */
  task t = worktodo[0];

  /* LSHF queue by 1. */
  for (int i = 1; i < nxt_pos; i++)
    worktodo[i - 1] = worktodo[i];
  nxt_pos--;

  return t;
}

/* The worker thread in the thread pool. */
void *worker(void *param) {
  /* Wait till work is available. */
  sem_wait(&full_queue);

  /* Wait for lock before accessing queue. */
  pthread_mutex_lock(&mutex);

  /* Critical section. */
  task tsk = dequeue();

  /* Release the lock. */
  pthread_mutex_unlock(&mutex);

  /* Execute the task now that work is available. */
  execute(tsk.function, tsk.data);

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
  task tsk = {somefunction, p};

  /* Wait for the lock before accessing queue. */
  pthread_mutex_lock(&mutex);

  /* Critical section. */
  int err = enqueue(tsk);

  /* Release the lock. */
  pthread_mutex_unlock(&mutex);
  if (err == 0)
    sem_post(&full_queue);

  return err;
}

/* Initialize the thread pool. */
void pool_init(void) {
  /* Init semaphore to keep track of the num of tasks in the queue. */
  sem_init(&full_queue, 0, 0); /* share only amoung threads in same proc */

  /* Init the mutex to lock access to queue. */
  pthread_mutex_init(&mutex, NULL);

  /* Create all threads. */
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_create(&bees[i], NULL, worker, NULL);
}

/* Shutdown the thread pool */
void pool_shutdown(void) {
  //
  pthread_join(bees[0], NULL);
}
