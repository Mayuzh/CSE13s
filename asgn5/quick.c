// quick.c - Contains the function definition for the Bubble Sort.
#include <stdint.h> // for uint32_t
#include <stdio.h>

#include "quick.h"
uint32_t moves = 0;
uint32_t compares = 0;
void swap(uint32_t *x, uint32_t *y) {
  uint32_t t = *x;
  *x = *y;
  *y = t;
}
void Partition(uint32_t *arr, uint32_t left, uint32_t right) {
  if (left >= right) {
    compares++;
    return;
  }
  uint32_t pivot = arr[left];
  uint32_t low = left + 1;
  uint32_t high = right;
  while (low < high) {
    compares++;
    while (arr[high] > pivot && low < high) {
      compares++;
      high--;
    }
    while (arr[low] <= pivot && low < high) {
      compares++;
      low++;
    }
    swap(&arr[low], &arr[high]);
    moves += 3;
  }
  if (arr[high] <= arr[left]) {
    compares++;
    swap(&arr[high], &arr[left]);
    moves += 3;
  } else {
    high--;
  }
  if (high) {
    Partition(arr, left, high - 1);
  }
  Partition(arr, high + 1, right);
  //return high;
}
void QuickSort(uint32_t *arr, uint32_t left, uint32_t right) {
  Partition(arr, left, right);
  printf("Quick Sort\n");
  printf("%d elements, %d moves, %d compares\n", right + 1, moves, compares);
  return;
}
