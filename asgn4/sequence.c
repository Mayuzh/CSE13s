#include "bv.h"
#include "sieve.h"
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITS_IN_BYTE 8
#define OPTIONS "spn:"

bool isIn(uint32_t *, uint32_t, uint32_t);
char num_to_char(uint32_t);
bool isPalindrome(char *, uint32_t);
void output_object(BitVector *, uint32_t, uint32_t);

// functions for intersting prime
bool isIn(uint32_t *arr, uint32_t n, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    if (arr[i] == n) {
      return true;
    }
  }
  return false;
}

//functions for palindromic primw
char num_to_char(uint32_t num) {
  if (num >= 0 && num <= 9)
    return (char)('0' + num - 0);
  else
    return (char)('a' + num - 10);
}
bool isPalindrome(char *temp, uint32_t len) {
  uint32_t j = len - 1;
  for (uint32_t i = 0; i < (len / 2); i += 1) {
    if (temp[i] != temp[j])
      return false;
    j -= 1;
  }
  return true;
}
void output_object(BitVector *sv, uint32_t number, uint32_t base) {
  printf("Base %2d\n", base);
  printf("---- --\n");
  char *temp;
  uint32_t size = (uint32_t)(
      log(number) / log(base)
      + 1); // log(number)/log(base)+1 elements needed for the number string
  temp = (char *)malloc(sizeof(char) * size);
  for (uint32_t n = 2; n < number; n += 1) {
    if (bv_get_bit(sv, n)) {
      uint32_t temp_n = n;
      uint32_t i = 0;
      while (temp_n > 0) {
        temp[i] = num_to_char(temp_n % base);
        temp_n = temp_n / base;
        i += 1;
      }
      temp[i] = '\0';
      if (isPalindrome(temp, i)) {
        printf("%d = %s\n", n, temp);
      }
    }
  }
  free(temp);
  return;
}

int main(int argc, char **argv) {
  if (argc
      == 1) // argc[0] is the executable filename, at least argc[1] is not NULL.
  {
    puts("Error: no arguments supplied!");
    return -1;
  }
  int c; // return value of getopt()
  uint32_t max = 1000;
  BitVector *S_v;
  bool isInteresting = false;
  bool isPalindrome = false;
  while ((c = getopt(argc, argv, OPTIONS))
         != -1) // if there are more than one option
  {
    switch (c) {
    case 'n':
      max = atoi(optarg);
      break;
    case 's':
      isInteresting = true;
      break;
    case 'p':
      isPalindrome = true;
      break;
    }
  }
  S_v = bv_create(max);
  sieve(S_v);
  if (isInteresting) {
    uint32_t *mer;
    uint32_t sizeMer = (uint32_t)(log(max) / log(2) + 1);
    mer = (uint32_t *)malloc(sizeof(uint32_t) * sizeMer);
    if (!mer)
      return 1;
    for (uint32_t i = 0; i < sizeMer; i++) {
      mer[i] = pow(2, i) - 1;
    }
    uint32_t *luc;
    uint32_t sizeLuc = (uint32_t)(log(max) / log(2) + 10);
    luc = (uint32_t *)malloc(sizeof(uint32_t) * sizeLuc);
    if (!luc)
      return 1;
    luc[0] = 2;
    luc[1] = 1;
    uint32_t i = 1;
    while (luc[i] < max) {
      i++;

      if (i == sizeLuc) {
        printf("sizeLuc: %d\n", sizeLuc);
        sizeLuc *= 2;
        luc = (uint32_t *)realloc(luc, sizeof(uint32_t) * sizeLuc);
      }

      luc[i] = luc[i - 1] + luc[i - 2];
    }
    sizeLuc = i;

    uint32_t *fib;
    uint32_t sizeFib = (uint32_t)(log(max) / log(2) + 10);
    fib = (uint32_t *)malloc(sizeof(uint32_t) * sizeFib);
    if (!fib)
      return 1;
    fib[0] = 0;
    fib[1] = 1;
    i = 1;
    while (fib[i] < max) {
      i++;
      if (i == sizeFib) {
        sizeFib *= 2;
        fib = (uint32_t *)realloc(fib, sizeof(uint32_t) * sizeFib);
      }
      fib[i] = fib[i - 1] + fib[i - 2];
    }
    sizeFib = i;

    for (uint32_t n = 2; n <= max; n++) {
      if (bv_get_bit(S_v, n)) {
        printf("%d: prime", n);
        if (isIn(mer, n, sizeMer)) {
          printf(", mersenne");
        }
        if (isIn(luc, n, sizeLuc)) {
          printf(", lucas");
        }
        if (isIn(fib, n, sizeFib)) {
          printf(", fibonacci");
        }
        printf("\n");
      }
    }
    free(mer);
    free(luc);
    free(fib);
  }
  if (isInteresting && isPalindrome) {
    printf("\n");
  }
  if (isPalindrome) {
    output_object(S_v, max, 2);
    printf("\n");
    output_object(S_v, max, 9);
    printf("\n");
    output_object(S_v, max, 10);
    printf("\n");
    // 'z' - 'a' + 10 = 36
    output_object(S_v, max, 36);
  }
  bv_delete(S_v);

  return 0;
}
