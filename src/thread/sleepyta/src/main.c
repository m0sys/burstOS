#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

#define NUM_CHAIRS 3

/* Global state shared between all threads. */
int is_ta_available = 1; /* boolean access ctrl by ta_lock mutex */
pthread_mutex_t ta_lock; /* mutex lock states whether ta is busy */
sem_t
    *num_chairs; /* semaphore keeping track of the num chairs outside of off. */

void *ta_worker(void *params);

void *student_worker(void *params);

int main(int argc, char **argv) {
  printf("Hello, World!\n");
  /* Read the num students given as input param. */
  if (argc == 1) {
    printf("Usage: ./main 10\n");
    return 0;
  }

  int n = atoi(argv[1]);

  /* Init TA and student threads. */
  pthread_t ta;
  pthread_create(&ta, NULL, ta_worker, NULL);

  pthread_t *s_workers = malloc(sizeof(pthread_t) * n); /* TODO: free */
  for (int i = 0; i < n; i++)
    pthread_create(&s_workers[i], NULL, student_worker, NULL);

  /* Init mutex and semaphores. */
  int err = pthread_mutex_init(&ta_lock, NULL);
  if (err) {
    printf("err: pthread_mutex_init!");
    return 0;
  }

  num_chairs = sem_open("NUM_CHAIR_SEM", O_CREAT, ALLPERMS, NUM_CHAIRS);
}
