#include <stdio.h>

/* The global sorting variables to be shared amoung all threads. */
int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

/* Sorting algs. */
void swap(int *arr, int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void insertion_sort(int *arr, int start, int end) {
  for (int i = start; i < end - 1; i++) {
    for (int j = i + 1; j > start; j--)
      if (arr[j - 1] > arr[j])
        swap(arr, j, j - 1);
  }
}

void selection_sort(int *arr, int start, int end) {
  for (int i = start; i < end; i++) {
    int min = arr[i];
    int min_idx = i;
    for (int j = i; j < end; j++)
      if (arr[j] < min) {
        min = arr[j];
        min_idx = j;
      }
    swap(arr, i, min_idx);
  }
}
