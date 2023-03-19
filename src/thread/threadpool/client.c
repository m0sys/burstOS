/**
 * Example client program that uses thread pool.
 */

#include "threadpool.h"
#include <stdio.h>
#include <unistd.h>

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
  struct data work;
  work.a = 5;
  work.b = 10;
  struct data work2;
  work2.a = 2;
  work2.b = 3;
  struct data work3;
  work3.a = 6;
  work3.b = 4;

  /* Initialize the thread pool. */
  pool_init();

  /* submit the work to the pool. */
  // int a = 2;
  // int b = 3;
  // int c = a + b;
  // printf("c = %d\n", c);
  // long int sum = 0;
  // for (int i = 1000000; i > 0; i--)
  //   sum++;
  /* NOTE: with enough computation between pool_init & pool_submit a seg fault
   * will occur.
   *
   * Perhaps this is because printf results in an interrupt? No its a matter
   * of how long till init and submit.
   *
   * NOTE: The issue will be solved once wait is called before calling execute
   * in the worker!
   */
  pool_submit(&add, &work);

  pool_submit(&add, &work2);
  pool_submit(&add, &work3);

  // may be helpful
  // sleep(3);

  pool_shutdown();

  return 0;
}
