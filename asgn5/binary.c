// binary.c - Contains the function definition for the Binary Insertion Sort.
#include <stdint.h> // for uint32_t
#include <stdio.h>

#include "binary.h"

void BinaryInsertionSort(uint32_t *arr, uint32_t size) {
  uint32_t moves = 0;
  uint32_t compares = 0;
  for (uint32_t i = 1; i < size;
       i++) // arr[0] to be first in the sorted array, the others are unsorted
  {
    compares++;
    // initialize the left and right
    uint32_t left = 0;
    uint32_t right = i;
    // binary search algorithm for putting elements in sorted array
    while (left < right) {
      compares++;
      uint32_t mid = left + (right - left) / 2;
      if (arr[i] >= arr[mid]) {
        compares++;
        left = mid + 1; // the value is in the left part
      } else {
        right = mid; // the value is in the right part
      }
    }
    for (uint32_t j = i; j > left; j--) {
      uint32_t tmp = arr[j];
      arr[j] = arr[j - 1];
      arr[j - 1] = tmp;
      moves += 3;
    }
   
  }
  printf("Binary Insertion Sort\n");
  printf("%d elements, %d moves, %d compares\n", size, moves, compares);
  return;
}
