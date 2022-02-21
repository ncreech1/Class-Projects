#include <stdlib.h>
#include "mymalloc.h"

/* Nicholas Creech
 * 11/02/20
 * Lab 6: Write a set of functions that mimic the functionality of malloc() and free()
 */

typedef struct flist
{
	int size;
	struct flist *prev;
	struct flist *next;
} Flist;

int compare(const void *p1, const void *p2);

Flist *head = NULL;

void * my_malloc(size_t size)
{
	Flist *current;
	Flist *prev;
	Flist *next;
	void *result;

	//Align memory request to multiple of 8 and add bookkeeping
	if(size % 8 != 0)
		size += (8 - size % 8) + 8;
	else
		size += 8;

	//Find memory in free list
	for(current = head; current != NULL; current = current->next)
	{
		//Free node has enough memory to use
		if(current->size >= size)
		{
			//Carve out part of free node only if at least 12 will be left
			if(current->size - size >= 12)
			{
				int oldSize;
				char *currentAddress;

				next = current->next;
				prev = current->prev;
				currentAddress = (char*)current;
				
				//Update size of current free node and shift forward in heap
				oldSize = current->size;
				current->size = size;
				result = currentAddress;
				currentAddress += size;
				current = (Flist*)currentAddress;
				current->size = oldSize - size;

				//Update node before current
				if(prev != NULL)
				{
					prev->next = current;
					current->prev = prev;
				}

				//Update head
				else
					head = (Flist*)current;

				//Update node after current
				if(next != NULL)
				{
					next->prev = current;
					current->next = next;
				}
			}

			//Use all of free node
			else
			{
				//Unlink back of current node
				if(current->prev != NULL)
					current->prev->next = NULL;
				//Update head
				else
					head = NULL;

				//Unlink front of current node
				if(current->next != NULL)
					current->next->prev = NULL;

				result = current;
			}

			return result + 8;
		}
	}

	//Get 8K memory from heap
	if(size <= 8192)
	{
		result = (void*)sbrk(8192);
		*(int*)result = 8192;
		my_free(result + 8);
		return my_malloc(size - 8);
	}

	//Get full memory from heap
	else
	{
		result = (void*)sbrk(size);
		*(int*)result = size;
		return result + 8;
	}
}

//Adds memory from heap to free list
void my_free(void *ptr)
{
	Flist *node;

	node = (Flist*)(ptr - 8);

	//Create head
	if(head == NULL)
	{
		head = node;
		node->next = NULL;
		node->prev = NULL;
	}

	//Create new node in list
	else
	{
		node->next = head;
		node->prev = NULL;
		head->prev = node;
		head = node;
	}
}

//Returns the free list head
void * free_list_begin()
{
	return head;
}

//Returns the node after a given node
void * free_list_next(void *node)
{
	return ((Flist*)node)->next;
}

//Comparison function for qsort() to sort addresses in ascending order
int compare(const void *p1, const void *p2)
{
	Flist *a = *((Flist**)p1);
	Flist *b = *((Flist**)p2);

	if(a < b)
		return -1;
	else if(a == b)
		return 0;
	else
		return 1;
}

//Combines contiguous memory in free list
void coalesce_free_list()
{
	Flist *current;
	Flist **freeList;
	int freeNodes;
	int freeListIndex;

	//Find total number of free nodes
	freeNodes = 0;
	for(current = head; current != NULL; current = current->next)
		freeNodes++;

	//Nothing to coalesce
	if(freeNodes == 0)
		return;

	//Create array of free node pointers
	freeList = (Flist**)malloc(sizeof(Flist*) * freeNodes);

	//Fill array of free node pointers
	freeListIndex = 0;
	for(current = head; current != NULL; current = current->next)
	{
		freeList[freeListIndex] = current;
		freeListIndex++;
	}

	//Sort array of free node pointers in ascending order
	qsort(freeList, freeNodes, sizeof(Flist*), compare);

	for(freeListIndex = 1; freeListIndex < freeNodes; freeListIndex++)
	{
		Flist *prev; //Previous node in sorted freeList, NOT the actual free list
		Flist *next; //Next node in sorted freeList, NOT the actual free list
		char *currentAddress;

		current = freeList[freeListIndex];
		prev = freeList[freeListIndex - 1];
		currentAddress = (char*)current;

		//Combine contiguous free memory
		if((Flist*)(currentAddress - prev->size) == prev)
		{
			prev->size += current->size;
			
			//Unlink back of current node
			if(current->prev != NULL)
				current->prev->next = current->next;
			
			//Update head
			else
			{
				head = current->next;
				current->next->prev = NULL;
			}

			//Unlink front of current node
			if(current->next != NULL)
				current->next->prev = current->prev;

			freeList[freeListIndex] = prev;
		}
	}

	free(freeList);
}
