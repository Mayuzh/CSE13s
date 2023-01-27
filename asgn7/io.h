#ifndef __IO_H__
#define __IO_H__

#include "word.h"
#include <inttypes.h>
#include <stdbool.h>

extern uint64_t total_syms;
extern uint64_t total_bits;

//
// Struct definition of a FileHeader.
//
// magic: Magic number indicating a file compressed by this program.
// protection: Protection/permissions of the original, uncompressed file.
//
typedef struct FileHeader{
	uint32_t magic;
	uint16_t protection;
} FileHeader;

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
int read_bytes(int infile, uint8_t *buf, int to_read);

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
int write_bytes(int outfile, uint8_t *buf, int to_write);

//
// Reads in a FileHeader from the input file.
//
// infile: File descriptor of input file to read header from.
// header: Pointer to memory where the bytes of the read header should go.
// returns: Void.
//
void read_header(int infile, FileHeader *header);

//
// Writes a FileHeader to the output file.
//
// outfile: File descriptor of output file to write header to.
// header: Pointer to the header to write out.
// returns: Void.
//
void write_header(int outfile, FileHeader *header);

//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym. (e.g. *sym = val)
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
// infile: File descriptor of input file to read symbols from.
// sym: Pointer to memory which stores the read symbol.
// returns: True if there are symbols to be read, false otherwise.
//
bool read_sym(int infile, uint8_t *tym);

//
// Buffers a pair. A pair is comprised of a code and symbol.
// The bits of the code are buffered first, starting from the LSB.
// The bits of the symbol are buffered next, also starting from the LSB.
// The code buffered has a bit-length of bitlen.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// sym: Symbol of the pair to buffer.
// index: Index of the pair to buffer.
// bit_len: Number of bits of the code to buffer.
// returns: Void.
//
void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len);

//
// Writes out any remaining pairs of symbols and codes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_pairs(int outfile);

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
bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bitlen);

//
// Buffers a Wrod, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w: Word to buffer.
// returns: Void.
//
void buffer_word(int outfile, Word *w);

//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_words(int outfile);

#endif



