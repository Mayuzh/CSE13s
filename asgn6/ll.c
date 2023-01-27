// ll.c contains the implementation of the linked list ADT.

#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// statistic information for command line option -s
extern struct Stat stats;

//
// Constructor for a ListNode.
//
// gs: HatterSpeak struct containing oldspeak and its hatterspeak translation.
//
ListNode *ll_node_create(HatterSpeak *gs)
{
	// create a new ListNode, allocate memory for newNode.
	ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
	if(!newNode) return (ListNode *)NIL; //return NULL if memory allocation fails.
	// initialize gs, allocate memory for gs.
	newNode->gs = (HatterSpeak *)malloc(sizeof(HatterSpeak));
	if(!newNode->gs) return (ListNode *)NIL;
	//printf("ll_node_create: going to malloc newNode->gs->oldspeak\n");
	//printf("ll_node_create: %s\n", gs->oldspeak);
	newNode->gs->oldspeak = (char *)malloc(sizeof(char) * (strlen(gs->oldspeak) + 10));
	strcpy(newNode->gs->oldspeak, gs->oldspeak);
	if(gs->hatterspeak != NULL)
	{
		newNode->gs->hatterspeak = (char *)malloc(sizeof(char) * (strlen(gs->hatterspeak) + 10));
		strcpy(newNode->gs->hatterspeak, gs->hatterspeak);
	}
	// initialize next.
	newNode->next = NIL;
	return newNode;
}

//
// Destructor for a ListNode.
//
// n: The ListNode to free.
//
void ll_node_delete(ListNode *n)
{
	free(n->gs->oldspeak);
	if(n->gs->hatterspeak != NULL) free(n->gs->hatterspeak);
	free(n->gs);
	n->gs = NIL;
	n->next = NIL;
	return;
}

// delete the entire linked list.
//
// head: The head of the linked list.
//
void ll_delete(ListNode *head)
{
	ListNode *tmp = head;
	ListNode *nxt;
	while(tmp != NIL)
	{
		nxt = tmp->next;
		ll_node_delete(tmp);
		free(tmp);
		tmp = nxt;
	}
	return;
}

//
// Creates and inserts a ListNode into a linked list.
//
// head: a reference (pointer to pointer) to the head of a list
// gs: HatterSpeak struct.
//

ListNode *ll_insert(ListNode **head, HatterSpeak *gs)
{
	// number of non-zero linked list nodes +1 when ll_insert is called
	stats.ll_total_len++;

	//create a new ListNode.
	ListNode *newNode = ll_node_create(gs);
	// link the old list off the new node.
	newNode->next = *head;
	// move the head to point to the new node.
	*head = newNode;
	// return the new head pointer.
	return *head;
}

// Searches for a specific key in a linked list.
// Returns the ListNode if found and NULL otherwise.

// head: The head of the linked list to search in.
// key: The key to search for.
ListNode *ll_lookup(ListNode **head, char *key)
{
    // each time ll_lookup be called, increase the statistics data -- links searched
    //printf("ll_lookup: stats.links_searched is %u\n", stats.links_searched);	
	stats.links_searched++;    	// dref head to get the real head.

	ListNode *preNode = *head;
	// if the first node in the head matches the key, return
	if(strcmp(key, preNode->gs->oldspeak) == 0) return preNode;
	ListNode *curNode = preNode->next;
    
	while(curNode != NIL)
	{
		// if move to check the next node in linked list, then increase the statistics data -- links searched
		stats.links_searched++;
		//printf("ll_lookup: stats.links_searched is %u\n", stats.links_searched);
		// check if the key string is same as the current_node->gs->oldspeak
		if(strcmp(key, curNode->gs->oldspeak) == 0)
		{
			if(move_to_front)
			{
				preNode->next = curNode->next;
				curNode->next = *head;
				*head = curNode;
			}
			return curNode;
		}
		// if not found, move to next ListNode, also keep tracking preNode.
		preNode = preNode->next;
		curNode = curNode->next;
		
	}
	return NULL;
}
