#include "board.h"
#include <stdio.h>

/* The global board variables shared by all threads. */
int board[N][N] = {0};
int bit_val[NUM_THREADS] = {0};

int is_valid_board() {
  for (int i = 0; i < NUM_THREADS; i++) {
    if (bit_val[i] == 0)
      return 0;
  }
  return 1;
}

void print_board() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      printf("%d ", board[i][j]);
    printf("\n");
  }
}
