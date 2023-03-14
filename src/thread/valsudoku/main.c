#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_attr_t.h>

#define NUM_THREADS 27
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

void *val_row_worker(void *param) {
  int sum = 0;
  int row = ((params *)param)->row;
  // printf("row: %d\n", row);

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
  printf("val_row_worker: row: %d -> sum = %d: exiting succesfully\n", row,
         sum);
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
  printf("val_col_worker: exiting succesfully\n");
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
  printf("val_square_worker: exiting succesfully\n");
  free(param);
  pthread_exit(0);
}

void print_board() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      printf("%d ", board[i][j]);
    printf("\n");
  }
}

int main(int argc, char **argv) {
  FILE *fp;
  /* NOTE: getline will automatically alloc required buffer to read line. */

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");

  int row = 0;
  /* Load board from file. */
  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Len line: %zu\n", read);
    printf("line: %s\n", line);
    int scnt = 0;
    for (int j = 0; j < read; j++) {
      if (line[j] == ' ') {
        scnt++;
        continue;
      }
      board[row][j - scnt] = atoi(&line[j]);
    }
    row++;
  }

  print_board();

  /* TODO: Unleash the threads. */
  pthread_t workers[NUM_THREADS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  /* Init row threads. */
  for (int i = 0; i < N; i++) {
    params *p = malloc(sizeof(params));
    p->row = i;
    p->col = 0;
    pthread_create(&workers[i], &attr, val_row_worker, p);
  }

  /* Init col threads. */
  for (int j = 0; j < N; j++) {
    params *p = malloc(sizeof(params));
    p->row = 0;
    p->col = j;
    pthread_create(&workers[j + 9], &attr, val_col_worker, p);
  }

  /* TODO: Init square threads. */
  /* 0x0 */
  params *pptr = malloc(sizeof(params));
  pptr->row = 0;
  pptr->col = 0;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 0x3 */
  pptr = malloc(sizeof(params));
  pptr->row = 0;
  pptr->col = 3;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 0x6 */
  pptr = malloc(sizeof(params));
  pptr->row = 0;
  pptr->col = 6;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 3x0 */
  pptr = malloc(sizeof(params));
  pptr->row = 3;
  pptr->col = 0;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 3x3 */
  pptr = malloc(sizeof(params));
  pptr->row = 3;
  pptr->col = 3;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 3x6 */
  pptr = malloc(sizeof(params));
  pptr->row = 3;
  pptr->col = 6;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 6x0 */
  pptr = malloc(sizeof(params));
  pptr->row = 6;
  pptr->col = 0;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 6x3 */
  pptr = malloc(sizeof(params));
  pptr->row = 6;
  pptr->col = 3;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  /* 6x6 */
  pptr = malloc(sizeof(params));
  pptr->row = 6;
  pptr->col = 6;
  pthread_create(&workers[pptr->row + (pptr->col / 3)], &attr,
                 val_square_worker, pptr);

  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(workers[i], NULL);

  printf("\n\n");
  for (int i = 0; i < NUM_THREADS; i++)
    printf("%d: %d\n", i, bit_val[i]);

  printf("\n\nIs valid board? %d\n", is_valid_board());

  fclose(fp);
  // free(line);
}
