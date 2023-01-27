// quick.h - Contains the function declarations for the Quick Sort.

#ifndef __QUICK_H__
#define __QUICK_H__

#include <inttypes.h>
#include <stdint.h> // for uint32_t
//
// Sort the given array of numbers using quick sort.
//
// arr: the array that stores numbers
//
void swap(uint32_t *, uint32_t *);
void Partition(uint32_t *, uint32_t, uint32_t);
void QuickSort(uint32_t *, uint32_t, uint32_t);

#endif
