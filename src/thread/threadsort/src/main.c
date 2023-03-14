#include "sort/sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3
#define ARR_SZ 10

/* The global sorting variables to be shared amoung all threads. */
int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

/* Sorting algs. */
void swap(int *arr, int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void insertion_sort(int *arr, int start, int end) {
  for (int i = start; i < end - 1; i++) {
    for (int j = i + 1; j > start; j--)
      if (arr[j - 1] > arr[j])
        swap(arr, j, j - 1);
  }
}

void selection_sort(int *arr, int start, int end) {
  for (int i = start; i < end; i++) {
    int min = arr[i];
    int min_idx = i;
    for (int j = i; j < end; j++)
      if (arr[j] < min) {
        min = arr[j];
        min_idx = j;
      }
    swap(arr, i, min_idx);
  }
}

void print_arr(int *arr, int start, int end) {
  for (int i = start; i < end; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

/* Workers. */
typedef struct {
  int start;
  int end;
  void (*sort_func)(int *, int, int);
} sort_params;

typedef struct {
  int start1;
  int end1;
  int start2;
  int end2;
  int sz;
  pthread_t tid1;
  pthread_t tid2;
} merge_params;

void *sort_worker(void *params) {
  sort_params *p = ((sort_params *)params);
  p->sort_func(arr, p->start, p->end);

  /* Cleanly exit the worker. */
  free(params);
  pthread_exit(0);
}

void *merge_worker(void *params) {
  merge_params *p = ((merge_params *)params);

  /* Only start merging once the two sorting threads are done. */
  pthread_join(p->tid1, NULL);
  pthread_join(p->tid2, NULL);

  /* Create aux array. */
  int *aux = malloc(sizeof(int) * p->sz);

  /* Copy the half sorted array into the aux array. */
  for (int i = 0; i < p->sz; i++)
    aux[i] = arr[i];

  for (int i = 0; i < p->sz; i++) {
    if (p->start1 > p->end1) /* lhs fully merged */
      arr[i] = aux[p->start2++];
    else if (p->start2 > p->end2) /* rhs fully merged */
      arr[i] = aux[p->start1++];
    else if (arr[p->start1] < arr[p->start2])
      arr[i] = arr[p->start1++];
    else
      arr[i] = arr[p->start2++];
  }

  /* Cleanly exit the worker. */
  free(aux);
  free(params);
  pthread_exit(0);
}

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
  int start2 = mid + 1;
  int end1 = mid;
  int end2 = ARR_SZ;

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
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(workers[i], NULL);

  print_arr(arr, 0, ARR_SZ);
}
