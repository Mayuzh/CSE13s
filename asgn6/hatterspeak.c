// hatterspeak.c contains main() and the other functions necessary to complete the assignment.

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>
#include <regex.h>

#include "bf.h"
#include "hash.h"
#include "ll.h"
#include "parser.h"
#include "speck.h"

#define OPTIONS "sh:f:mb"

struct Stat stats; // statistic information for command line option -s, defined in ll.h
bool move_to_front;

// output nontalk message
void nontalkMsg(char *err)
{
	printf("Dear Wonderlander,\n\n");
	printf("You have chosen to use words that the queen has decreed oldspeak.\n");
	printf("Due to your infraction you will be sent to the dungeon where you will be taught hatterspeak.\n\n");
	printf("Your errors:\n\n");
	printf("%s", err);
	return;
}
// output hatterspeak message
void hatterMsg(char *trans)
{
	printf("Dear Wonderlander,\n\n");
	printf("The decree for hatterspeak finds your message lacking. Some of the words that you used are not hatterspeak.\n");
	printf("The list shows how to turn the oldspeak words into hatterspeak.\n\n");
	printf("%s", trans);
	return;
}

// output complex message
void complexMsg(char *err, char *trans)
{
	printf("Dear Comrade,\n\n");
	printf("You have chosen to use words that the queen has decreed oldspeak.\n");
	printf("Due to your infraction you will be sent to the dungeon where you will be taught hatterspeak.\n\n");
	printf("Your errors:\n\n");
	printf("%s", err);
	printf("\n");
	printf("Appropriate hatterspeak translations.\n\n");
	printf("%s", trans);
	return;
}


int main(int argc, char **argv)
{
	uint32_t BF_SIZE = 0x100000;
    uint32_t HT_SIZE = 10000;
	if(argc == 1)
	{
		puts("Error: no arguments supplied!");
		return -1;
	}
	int c;
	bool show_stats = false;
	bool isM = false;
	bool isB = false;
	while((c = getopt(argc, argv, OPTIONS)) != -1)
	{
		switch(c)
		{
			case's':
				show_stats = true;
				break;
			case'h':
				HT_SIZE = atoi(optarg);
				break;
			case'f':
				BF_SIZE = atoi(optarg);
				break;
			case'm':
				isM = true;				
				break;
			case'b':
				isB = true;
				break;
		}
	}
	if(isM && isB) return -1;
        else if(isM) move_to_front = true;
	else if(isB) move_to_front = false;
	else return -1;	

	// initialize statistics data if need
	stats.seeks_total = 0;
	stats.links_searched = 0;
	stats.ll_total_len = 0;
	stats.load_HT = 0;
	stats.load_BF = 0;

	// allocate space for HatterSpeak struct, let gs point to NULL.
	HatterSpeak *gs = (HatterSpeak *)malloc(sizeof(HatterSpeak));	
	gs->oldspeak = NULL;
	gs->hatterspeak = NULL;
	// set Bloom Filter and Hash Table
	BloomFilter *bf = bf_create(BF_SIZE); 
	//printf("BF create\n");
	HashTable *ht = ht_create(HT_SIZE);
    //printf("HT create\n");
	FILE *fp;
	// read in a list of nonsense words.
    fp = fopen("oldspeak.txt", "r"); 
	while(!feof(fp))
	//for(int i = 0; i < 10; i++)
	{
		static char temp[128] = {0};
		fscanf(fp, "%s", temp);
		// set Bloom Filter for nonsense
		bf_insert(bf, temp);
		// set Hash Table for nonsense
		gs->oldspeak = temp;
		ht_insert(ht, gs);
		fscanf(fp, "\n");
	}
	fclose(fp);
	//printf("close oldspeak\n");
	// read in a space-separated list of oldspeak, hatterspeak pair.
	fp = fopen("hatterspeak.txt", "r"); 
	while(!feof(fp))
	//for(int i = 0; i < 10; i++)
	{
		static char key[128] = {0};
		static char value[128] = {0};
		fscanf(fp, "%s %s\n", key, value);
		// set Bloom Filter for nonsense
		bf_insert(bf, key);
		// set Hash Table for nonsense
		gs->oldspeak = key;
		gs->hatterspeak = value;
		ht_insert(ht, gs);
		fscanf(fp, "\n");
	}
	fclose(fp);
	//printf("close hatterspeak\n");
	// read text from standard input.
	fp = stdin;
	//fp = fopen("test.in", "r");
	char *pattern = "[A-Za-z0-9]+('|-)?[a-zA-Z0-9]+(-)?[0-9]*";
	uint32_t z, cflags = 0;
	regex_t reg;
	z = regcomp(&reg, pattern, cflags);
	if ( z != 0) return -1;
	uint32_t ERR_LEN = 4096; // size of string err
	uint32_t TRANS_LEN = 4096; // size of string trans
	char *word = next_word(fp, &reg); // from parser.c.
	char *err = (char *) malloc(sizeof(char) * ERR_LEN); // error message for oldspeak words.
	char *trans = (char *) malloc(sizeof(char) * TRANS_LEN); // translations message for hatterspeak words.
	while(!feof(fp))
	{
		if(word != NULL) // regular expression recognize it as a word
		{
			//printf("the word is: %s\n", word);
			if(bf_probe(bf, word)) // Blooom Filter hit
			{
				//printf("the word passed the BloomFilter: %s\n", word);
				ListNode *res = NULL;
				// calling ht_lookup() once, then the number of seeks +1
				stats.seeks_total++;
				if((res = ht_lookup(ht, word)) != NULL) // find the word in HashTable
				{
					// dynamically expand the string err as needed
					if(ERR_LEN - strlen(err) < 128)
					{
						ERR_LEN *= 2;
						err = (char *) realloc(err, sizeof(char) * ERR_LEN);
					}
					// dynamically expand the string trans as needed
					if(TRANS_LEN - strlen(trans) < 128)
					{
						TRANS_LEN *= 2;
						trans = (char *) realloc(trans, sizeof(char) * TRANS_LEN);
					}

					char *newline = "\n";
					char *arrow = " -> ";
					if(res->gs->hatterspeak == NULL) // it is a forbidden word
					{
						// put the forbidden word into string err = "oldspeak\noldspeak\n..."
						strcat(err, word); // oldspeak
						strcat(err, newline); // "\n"
					}
					else
					{
						// put the translation pair into string trans = "oldspeak -> hatterspeak\noldspeak -> hatterspeak\n..."
						strcat(trans, word);  // oldspeak
						strcat(trans, arrow); // " -> "
						strcat(trans, res->gs->hatterspeak); // hatterspeak
						strcat(trans, newline); // "\n"
					}
				}
			}
		
		}
		word = next_word(fp, &reg); // move to the next word
	}

	// output the message or statistic data
	if(show_stats)
	{
		for(uint32_t i = 0; i < BF_SIZE; i++)
		{
			stats.load_BF += bv_get_bit(bf->filter, i); // get the number of 1 in BloomFilter->BitVector
		}		
		printf("Seeks: %u\n", stats.seeks_total);
		printf("Average seek length: %f\n", stats.links_searched / (stats.seeks_total * 1.0));
		printf("Average Linked List Length: %f\n", stats.ll_total_len / (ht_count(ht) * 1.0));
		printf("Hash table load: %f\n", ht_count(ht) / (HT_SIZE * 1.0));
		printf("Bloom filter load: %f\n", stats.load_BF / (BF_SIZE * 1.0));
	}
	else
	{
		// output the message
		if(strlen(err) != 0 && strlen(trans) == 0) // just nontalk
		{
			nontalkMsg(err);
		}
		else if(strlen(err) == 0 && strlen(trans) != 0) // just oldspeak->hatterspeak
		{
			hatterMsg(trans);
		}
		else if(strlen(err) != 0 && strlen(trans) != 0) // both nontalk and old->hatter
		{
			complexMsg(err, trans);
		}
	}
	
	// release allocated memory
	free(err);
	free(trans);
	regfree(&reg);
	ht_delete(ht);
	bf_delete(bf);
	fclose(fp);
	return 0;
}
