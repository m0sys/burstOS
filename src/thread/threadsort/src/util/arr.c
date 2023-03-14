#include <stdio.h>

void print_arr(int *arr, int start, int end) {
  for (int i = start; i < end; i++)
    printf("%d ", arr[i]);
  printf("\n");
}
