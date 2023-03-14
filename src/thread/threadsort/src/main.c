#include "sort/sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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
  int *arr;
  void (*sort_func)(int *, int, int);
} sort_params;

typedef struct {
  int start1;
  int end1;
  int start2;
  int end2;
  int sz;
  int *arr;
} merge_params;

void sort_worker(void *params) {
  sort_params *p = ((sort_params *)params);
  p->sort_func(p->arr, p->start, p->end);

  /* Cleanly exit the worker. */
  free(params);
  pthread_exit(0);
}

void merge_worker(void *params) {
  merge_params *p = ((merge_params *)params);
  /* Create aux array. */
  int *aux = malloc(sizeof(int) * p->sz);

  /* Copy the half sorted array into the aux array. */
  for (int i = 0; i < p->sz; i++)
    aux[i] = p->arr[i];

  for (int i = 0; i < p->sz; i++) {
    if (p->start1 > p->end1) /* lhs fully merged */
      p->arr[i] = aux[p->start2++];
    else if (p->start2 > p->end2) /* rhs fully merged */
      p->arr[i] = aux[p->start1++];
    else if (p->arr[p->start1] < p->arr[p->start2])
      p->arr[i] = p->arr[p->start1++];
    else
      p->arr[i] = p->arr[p->start2++];
  }

  /* Cleanly exit the worker. */
  free(params);
  pthread_exit(0);
}

int main(void) {
  printf("Hello, World!\n");

  int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  print_arr(arr, 0, 10);

  /* Unleash the threads! */
}
