// trie.c contains implementation of Trie.

#include <stdio.h>
#include <stdlib.h>
#include "trie.h" 
#include "code.h"

//
// Constructor for a TrieNode.
//
// code: Code of the constructed TrieNode.
// returns: Pointer to a TrieNode that has been allocated memory.
//
TrieNode *trie_node_create(uint16_t code){
	TrieNode *newNode = (TrieNode *) malloc(sizeof(TrieNode));
	if(!newNode) return NULL;
	for(int i = 0; i < ALPHABET; i++){
		newNode->children[i] = NULL;
	}
	newNode->code = code;
	return newNode;
}

//
// Destructor for a TrieNode.
//
// n: TrieNode to free allocated memory for.
// returns: Void.
//
void trie_node_delete(TrieNode *n){
	free(n);
	n = NULL;
	return;
}

//
// Initializes a Trie: a root TrieNode with the code EMPTY_CODE.
//
// returns: Pointer to the root of a Trie.
//
TrieNode *trie_create(void){
	TrieNode *rootNode = trie_node_create(EMPTY_CODE);
	return rootNode;
}

//
// Resets a Trie to just the root TrieNode.
//
// root: Root of the Trie to reset.
// returns: Void.
//
void trie_reset(TrieNode *root){
	for(int i = 0; i < ALPHABET; i++)
	{
		if(root->children[i] != NULL)
		{
			trie_delete(root->children[i]);
		}
	}
	return;
}

//
// Deletes a sub_Trie starting from the sub-Trie's root.
//
// n: Root of the sub-Trie to delete.
// returns: Void.
//
void trie_delete(TrieNode *n)
{
	for(int i = 0; i < ALPHABET; i++)
	{
		if(n->children[i] != NULL)
		{
			trie_delete(n->children[i]);
		}
		else
		{
			continue;
		}
	}
	trie_node_delete(n);
	return;
}

//
// Returns a pointer to the child TrieNode representing the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
// n: TrieNode to step from.
// sym: Symbol to check for.
// returns: Pointer to the TrieNode representing the symbol.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym){
	return n->children[sym];
}


