// bubble.c - Contains the function definition for the Bubble Sort.
#include <stdint.h> // for uint32_t
#include <stdio.h>

#include "bubble.h"

// bubble sort: put two elements in a bubble, each move exchanges the elements in the bubble
void BubbleSort(uint32_t *arr, uint32_t size) {
  uint32_t moves = 0;
  uint32_t compares = 0;
  for (uint32_t i = 0; i < size;
       i++) // starts from the first element in the array
  {
    compares++;
    uint32_t j = size - 1;
    while (j > i) {
      compares++;
      if (arr[j] < arr[j - 1]) {
        uint32_t tmp = arr[j];
        arr[j] = arr[j - 1];
        arr[j - 1] = tmp;
        moves += 3;
      }
      j--;
    } // end-while: the smallest element ends up being in the first
  }
  printf("Bubble Sort\n");
  printf("%d elements, %d moves, %d compares\n", size, moves, compares);
  return;
}
