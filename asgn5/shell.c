// shell.c - Contains the function definition for the Bubble Sort.
#include <stdint.h> // for uint32_t
#include <stdio.h>

#include "shell.h"

void ShellSort(uint32_t *arr, uint32_t size) {
  uint32_t moves = 0;
  uint32_t compares = 0;
  uint32_t step = size * 5 / 11;
  ;
  while (step >= 1) {
    compares++;
    for (uint32_t i = step; i < size; i++) {
      compares++;
      for (uint32_t j = i; j > step - 1; j -= step) {
        compares++;
        if (arr[j] < arr[j - step]) {
          compares++;
          uint32_t tmp = arr[j];
          arr[j] = arr[j - step];
          arr[j - step] = tmp;
          moves += 3;
        }
      }
    }
    step = step * 5 / 11;
  }
  printf("Shell Sort\n");
  printf("%d elements, %d moves, %d compares\n", size, moves, compares);
  return;
}
