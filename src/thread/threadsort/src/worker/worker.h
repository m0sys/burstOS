#pragma once
#include <pthread.h>

/* Worker Params. */
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

void *sort_worker(void *params);
void *merge_worker(void *params);
