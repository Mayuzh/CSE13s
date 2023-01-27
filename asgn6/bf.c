// bf.c contains implementation of Bloom Filter.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bf.h"
#include "bv.h"
#include "speck.h"
//
// Constructor for a BloomFilter.
//
// size: The number of entries in the BloomFilter.
//
BloomFilter *bf_create(uint32_t size) 
{
	BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
	if(bf) 
	{
		bf->primary   [0] = 0xfc28ca6885711cf7;
		bf->primary   [1] = 0x2841af568222f773;
		bf->secondary [0] = 0x85ae998311115ae3;
		bf->secondary [1] = 0xb6fac2ae33a40089;
		bf->tertiary  [0] = 0xd37b01df0ae8f8d0;
		bf->tertiary  [1] = 0x911d454886ca7cf7;
		bf->filter = bv_create(size); // Creates a new BitVector of specified length, type(bv->fileter) = BitVector *.
		return bf;
	}
	return (BloomFilter *)NIL;
}

// The rest of the functions you must implement yourselves.
//
// Destructor for a BloomFilter.
//
// bf: The BloomFilter.
//
void bf_delete(BloomFilter *bf)
{
	bv_delete(bf->filter);
	free(bf);
	bf = NIL;
	return;
}

//
// Inserts a new key into the BloomFilter.
// Indices to set bits are given by the hash functions.
//
// bt: The BloomFilter.
// key: The key to insert into the BloomFilter.
//

void bf_insert(BloomFilter *bf, char *key)
{
	uint32_t index1 = hash(bf->primary, key) % bv_get_len(bf->filter);
	bv_set_bit(bf->filter, index1);
	
	uint32_t index2 = hash(bf->secondary, key) % bv_get_len(bf->filter);
	bv_set_bit(bf->filter, index2);
	
	uint32_t index3 = hash(bf->tertiary, key) % bv_get_len(bf->filter);
	bv_set_bit(bf->filter, index3);
	return;
}

//
// Probes a BloomFilter to check if a key has been inserted.
//
// bf: The BloomFilter.
// key: The key in which to check membership.
//

bool bf_probe(BloomFilter *bf, char *key)
{
	uint32_t index1 = hash(bf->primary, key) % bv_get_len(bf->filter);
	uint32_t index2 = hash(bf->secondary, key) % bv_get_len(bf->filter);
	uint32_t index3 = hash(bf->tertiary, key) % bv_get_len(bf->filter);

	bool in_1 = bv_get_bit(bf->filter, index1);
	bool in_2 = bv_get_bit(bf->filter, index2);
	bool in_3 = bv_get_bit(bf->filter, index3);
	//printf("bf_probe: the res of probe is: %d\n", in_1&&in_2&&in_3);
	return(in_1 && in_2 && in_3);
}
