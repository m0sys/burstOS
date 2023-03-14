#include "sort/sort.h"
#include "util/arr.h"
#include "worker/worker.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3
#define ARR_SZ 11

/* The extern global sorting variables to be shared amoung all threads. */
extern int arr[];

int main(void) {
  printf("Hello, World!\n");
  print_arr(arr, 0, ARR_SZ);

  /* Unleash the threads! */
  pthread_t workers[NUM_THREADS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  /* Init sorting threads. */
  int mid = ARR_SZ / 2;

  int start1 = 0;
  int start2 = mid;
  int end1 = mid;
  int end2 = ARR_SZ;
  printf("start1: %d, end1: %d\nstart2: %d, end2: %d\nmid: %d\n", start1, end1,
         start2, end2, mid);

  /* lhs sorter */
  sort_params *sp = malloc(sizeof(sort_params));
  sp->start = start1;
  sp->end = end1;
  sp->sort_func = insertion_sort;
  pthread_create(&workers[0], &attr, sort_worker, sp);

  /* rhs sorter */
  sp = malloc(sizeof(sort_params));
  sp->start = start2;
  sp->end = end2;
  sp->sort_func = insertion_sort;
  pthread_create(&workers[1], &attr, sort_worker, sp);

  /* Init merging thread. */
  merge_params *mp = malloc(sizeof(merge_params));
  mp->start1 = start1;
  mp->end1 = end1;
  mp->start2 = start2;
  mp->end2 = end2;
  mp->tid1 = workers[0];
  mp->tid2 = workers[1];
  mp->sz = ARR_SZ;
  pthread_create(&workers[2], &attr, merge_worker, mp);

  /* Wait on all threads before printing out the resulting sorted array. */

  printf("main: waiting for all threads ...\n");
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(workers[i], NULL);
  printf("main: done waiting for all threads!\n");

  print_arr(arr, 0, ARR_SZ);
}
