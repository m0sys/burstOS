#include "board/board.h"
#include "worker/worker.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_attr_t.h>

/* The global extern board variables shared by all threads. */
extern int board[N][N];
extern int bit_val[NUM_THREADS];

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

  /* Unleash the threads. */
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

  printf("\n\nIs valid board? %d\n", is_valid_board());

  fclose(fp);
}
