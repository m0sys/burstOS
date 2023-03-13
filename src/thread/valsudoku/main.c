#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 11
#define VAL_SUM 45
#define N 9

/* The global board variable shared by all threads. */
int board[N][N] = {0};
int bit_val[NUM_THREADS] = {0};

int is_valid_board() {
  for (int i = 0; i < NUM_THREADS; i++) {
    if (bit_val[i] == 0)
      return 0;
  }
  return 1;
}

/* Struct for passing params to threads. */
typedef struct {
  int row;
  int col;
} params;

void val_row_worker(void *param) {
  int sum = 0;
  int row = ((params *)param)->row;

  for (int j = 0; j < N; j++) {
    int val = board[row][j];
    if (val > 9 || val < 1)
      pthread_exit(0);
    sum += val;
  }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[row] = 1;
  pthread_exit(0);
}

void val_col_worker(void *param) {
  int sum = 0;
  int col = ((params *)param)->col;

  for (int i = 0; i < N; i++) {
    int val = board[i][col];
    if (val > 9 || val < 1)
      pthread_exit(0);
    sum += val;
  }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[col + 9] = 1;
  pthread_exit(0);
}
void val_square_worker(void *param) {
  int sum = 0;
  int col = ((params *)param)->col;
  int row = ((params *)param)->row;
  int max_i = row + 3;
  int max_j = col + 3;

  for (int i = row; i < max_i; i++)
    for (int j = col; j < max_j; j++) {
      int val = board[i][j];
      if (val > 9 || val < 1)
        pthread_exit(0);
      sum += val;
    }

  /* If valid sum set the bit to one. */
  if (sum == VAL_SUM)
    bit_val[row + (col / 3) + 18] = 1;
  pthread_exit(0);
}

int main(int argc, char **argv) {
  FILE *fp;
  /* NOTE: getline will automatically alloc required buffer to read line. */

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Len line: %zu\n", read);
    printf("line: %s\n", line);
    for (int i = 0; i < read; i++)
      printf("c = %c\n", line[i]);
  }

  fclose(fp);
  // free(line);
}
