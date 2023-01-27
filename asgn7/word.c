// word.c contains implementation of WordTable.

#include "word.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a word.
//
// syms: Array of symbols a Word represents.
// len: Length of the array of symbols.
// returns: Pointer to a Word that has been allocated memory.
//
Word *word_create(uint8_t *syms, uint64_t len) {
  Word *newWord = (Word *)malloc(sizeof(Word)); // Get a newWord.
  if (!newWord)
    return NULL; // Don't forget to verify if the malloc succeed.
  newWord->syms = (uint8_t *)calloc(
      len, sizeof(uint8_t)); // Get the syms for the newWord.
  if (!newWord->syms)
    return NULL; // Don't forget to verify if the malloc succeed.
  memcpy(newWord->syms, syms, len); // Make a deep copy of the *syms.
  newWord->len = len;
  return newWord;
}

//
// Constructs a new Word from the specified Word appended with a symbol.
// The Word specified to append to may be empty.
// If the above is the case, the new Word should contain only the symbol.
//
// w: Word to append to.
// sym: Symbol to append.
// returns: New Word which represents the result of appending.
//
Word *word_append_sym(Word *w, uint8_t sym) {
  // Prepare the syms for newWord.
  uint8_t *syms = (uint8_t *)calloc(w->len + 1, sizeof(uint8_t));
  if (!syms)
    return NULL; // Don't forget to verify if the malloc succeed.
  memcpy(syms, w->syms, w->len); // Copy the first w->len symbols.
  memcpy(syms + w->len, &sym, 1); // Copy the symbol to be appended.
  Word *newWord = word_create(syms, w->len + 1); // Create the newWord.
  free(syms); // Don't forget to release the temporary allocated memory.
  return newWord;
}

//
// Destructor for a Word.
//
// w: Word to free memory for.
// returns: Void.
//
void word_delete(Word *w) {
  free(w->syms);
  w->syms = NULL;
  free(w);
  w = NULL;
}

//
// Creates a new WordTable, which is an array of Words.
// A WordTable has a pre-defined size of MAX_CODE (UINT64_MAX - 1).
// This is because codes are 16-bit integers.
// A WordTable is initialized with a single Word at index EMPTY_CODE.
// This Word represents the empty word, a string of length of zero.
//
// returns: Initialized WordTable.
//
WordTable *wt_create(void) {
  //printf("wt_create is running\n");
  WordTable *newWT = (WordTable *)calloc(MAX_CODE, sizeof(WordTable));
  if (!newWT)
    return NULL;
  newWT[EMPTY_CODE] = word_create(NULL, 0);
  //printf("wt_create -- newWT[1]->syms: %s; newWT[1]->len: %u\n", newWT[1]->syms, newWT[1]->len);
  return newWT;
}

//
// Resets a WordTable to having just the empty Word.
//
// wt: WordTable to reset.
// returns: Void.
//
void wt_reset(WordTable *wt) {
  // Keep the empty Word, delete others.
  for (uint16_t i = EMPTY_CODE + 1; i < MAX_CODE; i++) {
    if (wt[i] != NULL) {
      word_delete(wt[i]);
    }
  }
  return;
}

//
// Deletes an entire WordTable.
// All Words in the WordTable must be deleted as well.
//
// wt: WordTable to free memory for.
// returns: Void.
//
void wt_delete(WordTable *wt) {
  for (uint16_t i = 0; i < MAX_CODE; i++) {
    if (wt[i] != NULL) {
      word_delete(wt[i]);
    }
  }
  free(wt);
  wt = NULL;
  return;
}
