// io.c contains implementation of read from the input file and write to the output file.

#include "io.h"
#include "code.h"
#include "word.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK 4096

// static global variable buffer
static uint8_t pairs[BLOCK] = { 0 }; // buffer to store binary pairs
static uint8_t symbols[BLOCK] = { 0 }; // buffer to store characters

//
// Wrapper for the read() syscall.
// Loops to read the specified number of bytes, or until input is exhausted.
// Returns the number of bytes read.
//
// infile:  File descriptor of the input file to read from.
// buf:     Buffer to store read bytes into.
// to_read: Number of bytes to read.
// returns: Number of bytes read.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
  //printf("read_bytes -- to_read is: %d\n", to_read);
  uint16_t counter = 0; // Number of bytes have been read.
  while (1) {
    int n = read(infile, buf + counter, to_read - counter); // Try to read.
    if (n < 0) // Get error code.
    {
      printf("Fail to read file!\n");
      return -1;
    } else if (n == 0) // Input is exhausted.
    {
      break;
    } else // Get part of the input.
    {
      counter += n; // Update the counter to counter + n.
      if (counter == to_read) // Finish reading.
      {
        break;
      }
    }
  }

  return counter; // Return the acctual number of bytes read.
}

//
// Wrapper for the write() syscall.
// Loops to write the specified number of bytes, or until nothing is written.
// Returns the number of bytes written.
//
// outfile:   File descriptor of the output file to write to.
// buf:       Buffer that stores the bytes to write out.
// to_write:  Number of bytes to write.
// returns:   Number of bytes written.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
  //printf("write_bytes -- to_write is: %d\n", to_write);
  int counter = 0; // Number of bytes have been written now.
  while (1) {
    int n = write(outfile, buf + counter,
        to_write - counter); // Attempt to write all the bytes.
    if (n < 0) // Get error code.
    {
      printf("Fail to write file!\n");
      return -1;
    } else if (n == 0) // Nothing is written.
    {
      break;
    } else // Write part of the bytes.
    {
      counter += n; // Update the counter to counter + n
      if (counter == to_write) // Finish writing.
      {
        break;
      }
    }
  }
  return counter; // Return the number of bytes acctually written.
}

//
// Reads in a FileHeader from the input file.
//
// infile: File descriptor of input file to read header from.
// header: Pointer to memory where the bytes of the read header should go.
// returns: Void.
//
void read_header(int infile, FileHeader *header) {
  //printf("read_header -- sizeof(FileHeader): %lu\n", sizeof(FileHeader));
  uint8_t *buf
      = (uint8_t *)calloc(1, sizeof(FileHeader)); // FileHeader occupies 8 Bytes
  if (!buf)
    return;
  int n = read_bytes(infile, buf, sizeof(FileHeader));
  if (n < 0)
    return;

  // the bellow two statements act as the same as the next two memcpy options
  //header->magic = *( (uint32_t *)buf );
  //header->protection = *( (uint16_t *)(buf + sizeof(uint32_t)) );
  memcpy(&header->magic, buf, sizeof(uint32_t));
  memcpy(&header->protection, buf + sizeof(uint32_t), sizeof(uint16_t));

  free(buf);

  return;
}

//
// Writes a FileHeader to the output file.
//
// outfile: File descriptor of output file to write header to.
// header: Pointer to the header to write out.
// returns: Void.
//
void write_header(int outfile, FileHeader *header) {
  uint8_t *buf = (uint8_t *)calloc(1, sizeof(FileHeader));
  if (!buf)
    return;
  memcpy(buf, &header->magic, sizeof(uint32_t));
  memcpy(buf + sizeof(uint32_t), &header->protection, sizeof(uint16_t));

  int n = write_bytes(outfile, buf, sizeof(FileHeader));
  if (n < 0)
    printf("write_header -- write outfile error!\n");
  free(buf);
  return;
}

//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym. (e.g. *sym = val)
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
bool read_sym(int infile, uint8_t *sym) {
  static int left_symbols = BLOCK; // How many symbols left unprocessed
  uint16_t index = total_syms % BLOCK; // Track the symbol to return
  // Read a BLOCK into buffer symbols at the beginning or when all symbols in buffer have been processed,
  if (index == 0) {
    left_symbols = read_bytes(infile, symbols, BLOCK);
    if (left_symbols < 0)
      return false;
    //printf("Call the read_bytes()!\nleft_symbols is: %u\n", left_symbols);

    // Show what in the buffer now.
    //for(int i = 0; i < left_symbols; i++)
    //{
    //	printf("symbols[%d] is %x\n", i, symbols[i]);
    //}
  }
  // If index is between 0~BLOCK, retrieve a symbol from buffer
  if (index < left_symbols) {
    *sym = symbols[index];
    total_syms++; // Increase the counter of total processed symbols.
    return true;
  }
  // Returns false if there are not symbols to read.
  return false;
}

//
// Clear all the bits in pairs[]. For the convenience of the bitwise or.
//
void clear_pairs(void) {
  for (uint16_t i = 0; i < BLOCK; i++) {
    pairs[i] = 0;
  }
  return;
}

//
// Buffers a pair. A pair is comprised of a code and symbol.
// The bits of the code are buffered first, starting from the LSB.
// The bits of the symbol are buffered next, also starting from the LSB.
// The code buffered has a bit-length of bitlen.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// code: Code of the pair to buffer.
// sym: Symbol of the pair to buffer.
// bit_len: Number of bits of the code to buffer.
// returns: Void.
//
void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  uint16_t index; // Put code + sym to which byte of pairs[]
  uint16_t bits; // Start from which bit of that byte (pairs[index])

  // Put code into the pairs first.
  //printf("Put the code into the pair\n");
  for (uint8_t i = 0; i < bit_len;
       i++) // Process each bit of code, start from LSB.
  {
    // Write to the output file whenever the buffer is full.
    // The size of the buffer is BLOCK * 8 bits.
    if (total_bits != 0 && total_bits % (BLOCK * 8) == 0) {
      //printf("call write_bytes\n");
      write_bytes(outfile, pairs, BLOCK);
      clear_pairs(); // Clear all the bits in pairs[].
    }
    // Put the pair (code,symbol) into the buffer.
    index = (total_bits / 8) % BLOCK; // Start from which byte of pairs[].
    bits = total_bits % 8; // Start from which bit of byte pairs[index].

    // If the ith bit (start from the left side) of code is 1, set the pairs[index].[bits].
    if ((0x01 << i) & code) {
      uint8_t mask = 0x01 << bits; // 0000 0001 << bits, set the bits-th bit.
      pairs[index] |= mask; // Set the bit.
    }
    total_bits++; // Increase the number of total bits processed.
  }

  // Put sym into the pairs secondly. Same as the above code segment.
  //printf("Put the symbol into the pair\n");
  for (uint8_t i = 0; i < 8; i++) {
    if (total_bits != 0 && total_bits % (BLOCK * 8) == 0) {
      write_bytes(outfile, pairs, BLOCK);
      clear_pairs(); // Clear all the bits in paris[].
    }

    index = (total_bits / 8) % BLOCK; // Start from which byte of pairs[].
    bits = total_bits % 8; // Start from which bit of byte pairs[index].

    // If the ith bit (start from the left side) of code is 1, set the pairs[index].[bits].
    if ((0x01 << i) & sym) {
      uint8_t mask = 0x01 << bits; // 0000 0001 << bits, set the bits-th bit.
      pairs[index] |= mask; // Set the bit.
    }
    total_bits++; // Increase the number of total bits processed.
  }

  return;
}

//
// Writes out any remaining pairs of symbols and codes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_pairs(int outfile) {
  // Each frame of buffer contains BLOCK * 8 bits.
  // total_bit % (BLOCK * 8) means number of bits left in the buffer.
  int bytes = floor((total_bits % (BLOCK * 8)) / 8) + 1;
  write_bytes(outfile, pairs, bytes);
  return;
}

//
// "Reads" a pair (code and symbol) from the input file.
// The "read" code is placed in the pointer to code (e.g. *code = val)
// The "read" symbol is placed in the pointer to sym (e.g. *sym = val)
// In reality, a block of pairs is read into a buffer.
// An index keeps track of the current bit in the buffer.
// Once all bits have been processed, antoher block is read.
// The first bitlen bits are the code, starting from the LSB.
// The last 8 bits of the pair are the symbol, starting from the LSB.
// Returns true if there are pairs left to read in the buffer, else false.
// There are pairs left to read if the read code is not STOP_CODE.
//
// infile: File descriptor of the input file to read from.
// code: Pointer to memory which stores the read code.
// sym: Pointer to memory which stores the read symbol.
// bitlen: Length in bits of the code to read.
// returns: True if there are pairs left to read, false otherwise.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {
  //printf("read_pair -- total_bits: %lu; bit_len: %u\n", total_bits, bit_len);

  // Get code from the pairs first. The code is 16 bits.
  static uint16_t index = 0; // Current bit in the buffer pairs[].
  uint16_t tmpcode = 0; // Keep the temporary code.
  for (uint8_t i = 0; i < bit_len;
       i++) // Process each bit of code, start from LSB.
  {
    // Read from the input file whenever the buffer is empty.
    if (index % (8 * BLOCK) == 0) {
      index = 0;
      int n = read_bytes(infile, pairs, BLOCK);
      if (n < 0)
        return false;
    }

    // Start from which byte of pairs[] -- index / 8
    // Start from which bit of byte pairs[index / 8] -- index % 8
    // If the bits-th bit of pairs[index] is 1, set the code.i.
    if ((0x01 << index % 8) & pairs[index / 8]) {
      uint16_t mask = 0x01 << i; // Needs a 16-bit mask as the code is 16 bits.
      tmpcode |= mask;
    }
    index++; // Increase the number of total bits processed.
  }
  *code = tmpcode;

  //printf("read_pair -- final code: %x\n", *code);

  // There is no pairs to read if the read code is STOP_CODE.
  if (*code == STOP_CODE) {
    return false;
  }

  // Get symbol from the pairs secondly. Same as above code segment.
  uint8_t tmpsym = 0; // Symbols are 8 bits.
  for (uint8_t i = 0; i < 8; i++) {
    // Read from the input file whenever the buffer is empty.
    if (index % (8 * BLOCK) == 0) {
      index = 0;
      int n = read_bytes(infile, pairs, BLOCK);
      if (n < 0)
        return false;
    }

    if ((0x01 << index % 8) & pairs[index / 8]) {
      uint8_t mask = 0x01 << i;
      tmpsym |= mask;
    }
    index++;
  }
  *sym = tmpsym;
  //printf("read_pair -- final sym: %x\n", *sym);

  // Read the pair (code, symbol) successfully, return true.
  return true;
}

//
// Buffers a Word, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w: Word to buffer.
// returns: Void.
//
void buffer_word(int outfile, Word *w) {
  // Make deep copy of the each symbol of a Word to symbols[].
  for (uint32_t i = 0; i < w->len; i++) {
    // Copy a symbol from Word *w to buffer symbols[].
    memcpy(&symbols[total_syms % BLOCK], &w->syms[i], 1);

    // Increase the number of symbols processed.
    total_syms++;
    // If the buffer symbols[] is full, write to the output file.
    // total_syms means the total counter of symbols processed.
    if (total_syms % BLOCK == 0) {
      //printf("buffer_word -- number of symbols: %lu; length of word: %u\n", total_syms, w->len);
      write_bytes(
          outfile, symbols, BLOCK); // uint8_t symbols[BLOCK] is the buffer.
    }
  }

  return;
}

//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_words(int outfile) {
  uint16_t left_bytes = total_syms % BLOCK;
  write_bytes(outfile, symbols, left_bytes);
  return;
}
