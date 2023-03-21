/**
 * Example client program that uses thread pool.
 */

#include "threadpool.h"
#include <stdio.h>
#include <unistd.h>

#define WORK_LOAD 10000

struct data {
  int a;
  int b;
};

void add(void *param) {
  struct data *temp;
  temp = (struct data *)param;

  printf("I add two values %d and %d result = %d\n", temp->a, temp->b,
         temp->a + temp->b);
}

int main(void) {
  /* Create some work to do. */
  struct data works[WORK_LOAD];

  for (int i = 1; i <= WORK_LOAD; i++) {
    works[i - 1].a = i - 1;
    works[i - 1].b = i;
  }

  /* Initialize the thread pool. */
  pool_init();

  for (int i = 0; i < WORK_LOAD; i++)
    pool_submit(&add, &works[i]);

  /* may be helpful */
  sleep(3);

  pool_shutdown();

  return 0;
}
