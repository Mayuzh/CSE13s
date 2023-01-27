// encode.c contains the main function of the encoder.

#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#define OPTIONS "vi:o:"
#define MAGIC 0x8badbeef // The magic number is verified when a file is opened.

uint64_t total_syms = 0;
uint64_t total_bits = 0;

// Compute the bit-length of code
uint8_t bit_len(uint16_t code) {
  if (code == 0) // Avoid illegal log(0).
  {
    return 1;
  } else {
    // Code starts from 2: bit_len(2) = 2; bit_len(3) = 2; bit_len(4) = 3; ...
    return floor(log(code) / log(2)) + 1;
  }
}

// LZ78 decompression function.
void decompress(int infile, int outfile) {
  //printf("Decompress -- decompress is running!\n");
  // Create a WordTable.
  WordTable *table = wt_create();
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  // Declare a monotonic counter to keep track of the next available code.
  uint16_t next_code = START_CODE;

  // Loop to read in all the pairs from compressed infile.
  while (read_pair(infile, &curr_code, &curr_sym, bit_len(next_code))) {
    table[next_code] = word_append_sym(table[curr_code], curr_sym);
    buffer_word(outfile, table[next_code]);
    next_code++;
    if (next_code == MAX_CODE) {
      //printf("wt_reset!\n");
      wt_reset(table);
      next_code = START_CODE;
    }
  }
  flush_words(outfile); // Write the Words left in buffer to outfile.
  wt_delete(table); // Release the memory allocated to WordTable.
  return;
} // end function decompress

int main(int argc, char **argv) {

  // Get command line options. Default stdin and stdout are used if no arguments provided.
  int c; // Return value of getopt().
  int infile = STDIN_FILENO; // Linux standard input device file No. -- keyboard
  int outfile
      = STDOUT_FILENO; // Linux standart output device file No. -- screen
  bool isVerbose;
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'v':
      isVerbose = true;
      break;
    case 'i':
      //printf("infile name is: %s\n", optarg);
      infile = open(optarg, O_RDONLY);
      if (infile == -1) {
        printf("open input file %s failed!\n", optarg);
        return -1;
      }
      break;
    case 'o':
      //printf("outfile name is: %s\n", optarg);
      // open outfile -- write-only, create it if not exist, clear it
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0755);
      if (outfile == -1) {
        printf("open output file %s failed!\n", optarg);
        return -1;
      }
      break;
    }
  }

  // Initialize FileHeader.
  FileHeader *header = (FileHeader *)malloc(sizeof(FileHeader));
  struct stat st; // stat -- statistics of the file

  // Test for read_header.
  read_header(infile, header);
  fstat(infile, &st);
  //printf("the size of input file is: %ld\n", st.st_size);

  if (header->magic != MAGIC) {
    printf("The input file's magic number is: %x, which doesn't match the "
           "MAGIC!\n",
        header->magic);
    return -1;
  }

  // Test for decompress function.
  decompress(infile, outfile);
  struct stat st_out;
  fstat(outfile, &st_out);
  fchmod(outfile, st.st_mode);
  if (isVerbose == true) {
    printf("Compressed file size: %ld bytes\n", st.st_size);
    printf("Uncompressed file size: %ld bytes\n", st_out.st_size);
    double temp = (double)st.st_size / st_out.st_size;
    double ratio = 100 * (1 - temp);
    printf("Compression ratio: %.2lf%c\n", ratio, '%');
  }
  free(header);
  header = NULL;
  close(infile);
  close(outfile);
  return 0;
}
