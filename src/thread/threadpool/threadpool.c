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

#define NUM_THREADS 3

#define TRUE 1

/* This represents work that has to be completed by a thread in the pool. */
typedef struct {
  void (*function)(void *p);
  void *data;
} task;

/* Queue node. */
struct node {
  struct node *nxt;
  task task;
};

typedef struct node node;

node *head = NULL;
node *last = NULL;

/**
 * All the global mutexes/semaphors shared between threads.
 */

/* Global timeout for sem_wait. */
struct timespec ts;

/* Keep track of how full the queue is. */
sem_t *full_queue;

/* A mutex lock for locking access to queue. */
pthread_mutex_t mutex;

/* The worker bee. */
pthread_t bees[NUM_THREADS];

/*
 * Insert a task into the queue.
 *
 * Returns 0 if successful or 1 otherwise.
 */
int enqueue(task t) {
  /* Create new node. */
  node *new_node = malloc(sizeof(node));
  new_node->task.data = t.data;
  new_node->task.function = t.function;

  /* Append to end of queue. */
  if (head == NULL) {
    head = new_node;
    last = new_node;
  } else {
    last->nxt = new_node;
    last = last->nxt;
  }

  return 0;
}

/* Remove a task from the queue. */
task dequeue() {
  /* FIFO. */
  node *old_head = head;
  head = head->nxt;
  if (last == old_head)
    last = head;

  task t = old_head->task; /* copy task */
  free(old_head);
  return t; /* ret copied task */
}

/* The worker thread in the thread pool. */
void *worker(void *param) {
  while (1) {
    pthread_testcancel();

    /* Wait till work is available. */
    int err = sem_wait(full_queue);
    if (err)
      printf("worker: sem_wait(err) -> %d\n", err);

    /* Wait for lock before accessing queue. */
    err = pthread_mutex_lock(&mutex);
    if (err)
      printf("worker: mutex_lock(err) -> %d\n", err);

    /* Critical section. */
    task tsk = dequeue();

    /* Release the lock. */
    pthread_mutex_unlock(&mutex);
    if (err)
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
  int err = pthread_mutex_lock(&mutex);
  if (err)
    printf("pool_submit: mutex_lock(err) -> %d\n", err);

  /* Critical section. */
  err = enqueue(tsk);
  if (err)
    printf("pool_submit: enqueue(err) -> %d\n", err);

  if (err == 1) {
    err = pthread_mutex_unlock(&mutex);
    if (err)
      printf("pool_submit: mutex_unlock(err) -> %d\n", err);
    return 1;
  }

  /* Release the lock. */
  err = pthread_mutex_unlock(&mutex);
  if (err)
    printf("pool_submit: mutex_unlock(err) -> %d\n", err);

  if (err == 0) {
    err = sem_post(full_queue);
    if (err)
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
  if (err)
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
  if (err)
    printf("pool_shutdown: sem_close(err) -> %d\n", err);
  err = sem_unlink("FULL_QUEUE_SEM");
  if (err)
    printf("pool_shutdown: sem_unlink(err) -> %d\n", err);
}
