#pragma once

#define NUM_THREADS 27
#define VAL_SUM 45
#define N 9

/* Struct for passing params to threads. */
typedef struct {
  int row;
  int col;
} params;

int is_valid_board();
void print_board();
