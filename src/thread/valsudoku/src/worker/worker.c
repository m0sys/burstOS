#include "worker.h"

#include <pthread.h>
#include <stdlib.h>

/* The global extern board variables shared by all threads. */
extern int board[N][N];
extern int bit_val[NUM_THREADS];

void *val_row_worker(void *param) {
  int sum = 0;
  int row = ((params *)param)->row;

  for (int j = 0; j < N; j++) {
    int val = board[row][j];
    if (val > 9 || val < 1) {
      free(param);
      pthread_exit(0);
    }
    sum += val;
  }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[row] = 1;
  free(param);
  pthread_exit(0);
}

void *val_col_worker(void *param) {
  int sum = 0;
  int col = ((params *)param)->col;

  for (int i = 0; i < N; i++) {
    int val = board[i][col];
    if (val > 9 || val < 1) {
      free(param);
      pthread_exit(0);
    }
    sum += val;
  }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[col + 9] = 1;
  free(param);
  pthread_exit(0);
}

void *val_square_worker(void *param) {
  int sum = 0;
  int col = ((params *)param)->col;
  int row = ((params *)param)->row;
  int max_i = row + 3;
  int max_j = col + 3;

  for (int i = row; i < max_i; i++)
    for (int j = col; j < max_j; j++) {
      int val = board[i][j];
      if (val > 9 || val < 1) {
        free(param);
        pthread_exit(0);
      }
      sum += val;
    }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[row + (col / 3) + 18] = 1;
  free(param);
  pthread_exit(0);
}
