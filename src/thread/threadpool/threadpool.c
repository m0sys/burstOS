/**
 * Implementation of thread pool.
 */

#include "threadpool.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/semaphore.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define QUEUE_SIZE 100
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

/* Global timeout for sem_wait. */
struct timespec ts;

/* Keep track of how full the queue is. */
sem_t *full_queue;

/* A mutex lock for locking access to queue. */
pthread_mutex_t mutex;

/* The work queue. */
task worktodo[QUEUE_SIZE];

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
  printf("enqueue: nxt_pos=%d\n", nxt_pos);

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
  while (1) {
    pthread_testcancel();

    /* Wait till work is available. */
    int err = sem_wait(full_queue);
    printf("worker: sem_wait(err) -> %d\n", err);

    /* Wait for lock before accessing queue. */
    printf("worker: waiting for mutex ...\n");
    err = pthread_mutex_lock(&mutex);
    printf("worker: mutex_lock(err) -> %d\n", err);

    /* Critical section. */
    task tsk = dequeue();

    /* Release the lock. */
    pthread_mutex_unlock(&mutex);
    printf("worker: mutex_unlock(err) -> %d\n", err);

    /* Execute the task now that work is available. */
    execute(tsk.function, tsk.data);
  }
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
  printf("pool_submit: waiting for mutex ...\n");
  int err = pthread_mutex_lock(&mutex);
  printf("pool_submit: mutex_lock(err) -> %d\n", err);

  /* Critical section. */
  err = enqueue(tsk);
  printf("pool_submit: enqueue(err) -> %d\n", err);

  if (err == 1) {
    printf("\n\npool_submit: queue is full!!!\n\n");
    err = pthread_mutex_unlock(&mutex);
    printf("pool_submit: mutex_unlock(err) -> %d\n", err);
    return 1;
  }

  /* Release the lock. */
  err = pthread_mutex_unlock(&mutex);
  printf("pool_submit: mutex_unlock(err) -> %d\n", err);

  if (err == 0) {
    printf("pool_submit: signaling threads of new available work ...\n");
    err = sem_post(full_queue);
    printf("pool_submit: sem_post(err) -> %d\n", err);
  }

  return err;
}

/* Initialize the thread pool. */
void pool_init(void) {
  /* Init the timer to 5 sec max wait. */
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 5;

  /* Init semaphore to keep track of the num of tasks in the queue. */
  full_queue = sem_open("FULL_QUEUE_SEM", O_CREAT, ALLPERMS, 0);

  /* Init the mutex to lock access to queue. */
  int err = pthread_mutex_init(&mutex, NULL);
  printf("pool_init: mutex_intit(err) -> %d\n", err);

  /* Create all threads. */
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_create(&bees[i], NULL, worker, NULL);
}

/* Shutdown the thread pool */
void pool_shutdown(void) {
  /* Signal all threads to cancel themselves. */
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_cancel(bees[i]);

  /* Wait for all threads to exit. */
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(bees[i], NULL);

  /* Clean up sem. */
  int err = sem_close(full_queue);
  printf("pool_shutdown: sem_close(err) -> %d\n", err);
  err = sem_unlink("FULL_QUEUE_SEM");
  printf("pool_shutdown: sem_unlink(err) -> %d\n", err);
}
