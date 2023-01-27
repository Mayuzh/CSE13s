// sorting.c contains main() and may contain any other functions necessary to complete theassignment.
//
// standard header file
#include <getopt.h> // for command line arguments
#include <stdbool.h> // for bool
#include <stdint.h> // for uint32_t
#include <stdio.h>
#include <stdlib.h> // for srand
//
// sources files of function declarations and macro definitions
#include "binary.h"
#include "bubble.h"
#include "quick.h"
#include "shell.h"

#define OPTIONS "Absqip:r:n:"

//uint32_t *arr; // global pointer for array, can be accessed by any funcitons in this project
void arrayGen(uint32_t *, uint32_t, uint32_t);
void output(uint32_t *, uint32_t);

// generate array of random numbers
// with the same random seed, the numbers would be the same for each-time function call
void arrayGen(uint32_t *arr, uint32_t seed, uint32_t size) {
  //arr = (uint32_t *) malloc(sizeof(uint32_t) * size); // dynamically allocate memory for array
  if (!arr)
    return; // check if allocation succeed
  srand(seed); // set up the random seed
  uint32_t mask = 0x3fffffff; // set up the bit mask
  for (uint32_t i = 0; i < size; i++) {
    arr[i] = rand()
             & mask; // bit masking to keep the number no larger than 30 bits
  }
  return;
}
void output(uint32_t *arr, uint32_t size) {
  uint32_t counter = 0;
  for (uint32_t i = 0; i < size; i++) {
    printf("%13d", arr[i]);
    counter++;
    if (counter == 7) {
      printf("\n");
      counter = 0;
    }
  }
  printf("\n");
}

int main(int argc, char **argv) {
  int c; // return value of getopt()
  bool isBinary; // true if binary insertion sort is enabled
  bool isBubble;
  bool isShell;
  bool isQuick;

  uint32_t printNum = 100; // default value for the first n elements to print
  uint32_t ranSeed = 8222022; // default value for random seed
  uint32_t arrSize = 100; // default value for array size

  // get command line options and arguments
  if (argc
      == 1) // argc[0] is the executable filename, at least argc[1] is not NULL
  {
    puts("Error: no arguments supplied!");
    return -1;
  }
  // parse the command line options & arguments
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'A':
      isBubble = true;
      isShell = true;
      isQuick = true;
      isBinary = true;
      break;
    case 'b':
      isBubble = true;
      break;
    case 's':
      isShell = true;
      break;
    case 'q':
      isQuick = true;
      break;
    case 'i':
      isBinary = true;
      break;
    case 'p':
      printNum = atoi(optarg); // atoi(char s[]) convert string to integer
      break;
    case 'r':
      ranSeed = atoi(optarg); // atoi(char s[]) convert string to integer
      break;
    case 'n':
      arrSize = atoi(optarg); // atoi(char s[]) convert string to integer
      break;
    } // end-switch
  } // end-while
  //arrayGen(ranSeed, arrSize);
  if (printNum > arrSize) {
    printNum = arrSize;
  }
  uint32_t *arr;
  arr = (uint32_t *)malloc(
      sizeof(uint32_t) * arrSize); // dynamically allocate memory for array
  if (isBinary) {
    arrayGen(arr, ranSeed, arrSize);
    BinaryInsertionSort(arr, arrSize);
    output(arr, printNum);
  }
  if (isQuick) {
    arrayGen(arr, ranSeed, arrSize);
    QuickSort(arr, 0, arrSize - 1);
    output(arr, printNum);
  }
  if (isShell) {
    arrayGen(arr, ranSeed, arrSize);
    ShellSort(arr, arrSize);
    output(arr, printNum);
  }
  if (isBubble) {
    arrayGen(arr, ranSeed, arrSize);
    BubbleSort(arr, arrSize);
    output(arr, printNum);
  }
  free(arr);

  return 0;
}
