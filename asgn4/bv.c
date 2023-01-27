#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "sieve.h"
#include "bv.h"
//
// Creates a new BitVector of specified length.
//
// bit_len : The length in bits of the BitVector.
//
BitVector *bv_create(uint32_t bit_len) {
  BitVector *v = (BitVector *)malloc(sizeof(BitVector));
  if (!v)
    return 0;
  v->length = bit_len;
  uint32_t byte_len = bit_len / 8 + 1;
  // allocate to vector byte_len blocks of 8 bits each
  v->vector = (uint8_t *)malloc(sizeof(uint8_t) * byte_len);
  if (!v->vector)
    return 0;

  return v;
}
//
// Frees memory allocated for a BitVector.
//
// v : The BitVector.
//
void bv_delete(BitVector *v) {
  free(v->vector);
  free(v);
  return;
}
//
// Returns the length in bits of the BitVector.
//
// v : The BitVector.
//
uint32_t bv_get_len(BitVector *v) {
  return v->length;
}
//
// Sets the bit at index in the BitVector.
//
// v : The BitVector.
// i : Index of the bit to set.
//
void bv_set_bit(BitVector *v, uint32_t i) {
  uint8_t i_line = 7 - (i % 8);
  v->vector[i / 8] |= 0x01 << i_line;
  return;
}
//
// Clears the bit at index in the BitVector.
//
// v : The BitVector.
// i : Index of the bit to clear.
//
void bv_clr_bit(BitVector *v, uint32_t i) {
  uint8_t i_line = 7 - (i % 8);
  v->vector[i / 8] &= ~(0x01 << i_line);
  return;
}
//
// Gets a bit from a BitVector.
//
// v : The BitVector.
// i : Index of the bit to get.
//
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  // i_line: the index of the bit for each line: starts from 0 to 7
  uint8_t i_line = 7 - (i % 8);
  if ((v->vector[i / 8] & (0x01 << i_line)) > 0) {
    return 1;
  } else {
    return 0;
  }
}
//
// Sets all bits in a BitVector.
//
// v : The BitVector.
//
void bv_set_all_bits(BitVector *v) {
  uint32_t i = 0;
  uint8_t init_num = 0xff;
  while (i < (v->length / 8 + 1)) {
    v->vector[i] = init_num;
    i++;
  }
}
