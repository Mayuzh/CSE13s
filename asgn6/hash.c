// hash.c contains implenetation of the hash table ADT.

#include "hash.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>


//
// Constructor for a HashTable.
//
// length: Length of the HashTable.
// salt: Salt for the HashTable.
//
HashTable *ht_create(uint32_t length) {
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	if (ht) {
		ht->salt[0] = 0x85ae998311115ae3;
		ht->salt[1] = 0xb6fac2ae33a40089;
		ht->length = length;
		ht->heads = (ListNode **)calloc(length, sizeof(ListNode *));
		return ht;
	}	
	
	return (HashTable *)NIL;
}

// The rest of the functions you must implement yourselves.
//
// Destructor for a HashTable.
//
// ht: The HashTable.
//
void ht_delete(HashTable *ht)
{
	// for loop to delete every struct exists in linked list
	for(uint32_t i = 0; i < ht->length; i++)
	{
		if(ht->heads[i] != NULL)
		{
			ll_delete(ht->heads[i]);
			ht->heads[i] = (ListNode *)NIL;
		}
	}
	// release the memory allocated for heads array.
	free(ht->heads);
	ht->heads = (ListNode **)NIL;
	// release the memory allocated for ht
	free(ht);
	ht = (HashTable *)NIL;
	return;
}

//
// Returns number of entries in hash table
//
// h: The HashTable.
//
uint32_t ht_count(HashTable *h)
{
	uint32_t counter = 0;
	for(uint32_t i = 0; i < h->length; i++)
	{
		if(h->heads[i] != NULL)
		{
			counter++;
		}
	}
	return counter;
}

//
// Searches a HashTable for a key.
// Returns the ListNode if found and returns NULL otherwise.
// This should call the ll_lookup() function.
//
// ht: The HashTable.
// key: The key to search for.
//
ListNode *ht_lookup(HashTable *ht, char *key)
{
	//printf("ht_lookup\n");
	// get the hash index using oldspeak as the key.
	uint32_t index = hash(ht->salt, key) % ht->length;
	// initialize the pointer 
	ListNode *head = ht->heads[index];
	// let head pointer point to the address of array at index
	// call the ll_lookup() function
	if(head != NIL)
	{
		return ll_lookup(&head, key);
	}
	return NULL;
}

//
// First creates a new ListNode from HatterSpeak.
// The created ListNode is then inserted into a HashTable.
// This should call the ll_insert() function.
//
// ht: The HashTable.
// gs: The HatterSpeak to add to the HashTable.
//
void ht_insert(HashTable *ht, HatterSpeak *gs)
{
	// get the hash index using oldspeak as the key.
	uint32_t index = hash(ht->salt, gs->oldspeak) % ht->length;
	// initialize the pointer 
	//ListNode *head = NULL;
	// let head pointer point to the address of array at index
	//head = ht->heads[index];
	ll_insert(&(ht->heads[index]), gs);
	//printf("ht_insert: %u -- (%s -> %s)\n", index, ht->heads[index]->gs->oldspeak, ht->heads[index]->gs->hatterspeak);
	return;
}
