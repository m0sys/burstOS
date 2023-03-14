#include "worker.h"
#include "../util/arr.h"

#include <stdio.h>
#include <stdlib.h>

/* The extern global sorting variables to be shared amoung all threads. */
extern int arr[];

void *sort_worker(void *params) {
  sort_params *p = ((sort_params *)params);
  p->sort_func(arr, p->start, p->end);

  /* Cleanly exit the worker. */
  printf("sort_worker: start: %d => exiting ...\n", p->start);
  print_arr(arr, p->start, p->end);
  free(params);
  pthread_exit(0);
}

void *merge_worker(void *params) {
  merge_params *p = ((merge_params *)params);

  /* Only start merging once the two sorting threads are done. */
  printf("merge_worker: waiting for sorting threads ...\n");
  pthread_join(p->tid1, NULL);
  pthread_join(p->tid2, NULL);
  printf("merge_worker: done waiting for sorting threads!\n");
  print_arr(arr, p->start1, p->end2);

  /* Create aux array. */
  int *aux = malloc(sizeof(int) * p->sz);

  /* Copy the half sorted array into the aux array. */
  for (int i = 0; i < p->sz; i++)
    aux[i] = arr[i];

  print_arr(aux, p->start1, p->end2);

  for (int i = 0; i < p->sz; i++) {
    if (p->start1 >= p->end1) /* lhs fully merged */
      arr[i] = aux[p->start2++];
    else if (p->start2 >= p->end2) /* rhs fully merged */
      arr[i] = aux[p->start1++];
    else if (aux[p->start1] < aux[p->start2])
      arr[i] = aux[p->start1++];
    else
      arr[i] = aux[p->start2++];
  }

  /* Cleanly exit the worker. */
  free(aux);
  free(params);
  pthread_exit(0);
}
