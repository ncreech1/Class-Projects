#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

/* Nicholas Creech
 * CS365 HW5
 * 03/04/21
 */ 

typedef struct queueStruct
{
	int size;
	int front;
	int back;
	void * innerArray[];
} Queue;

//Creates a new, empty queue
void * queue_new(int numItems)
{
	Queue *newQueue = malloc(sizeof(Queue) + (sizeof(void *) * numItems));	
	newQueue->size = numItems;
	newQueue->front = -1;
	newQueue->back = -1;
	return newQueue;
}

//Adds a new item to the back of the queue
void queue_enqueue(void *q, void *item)
{
	Queue *qp = (Queue*)q;
	
	//Queue is full	
	if(qp->front != -1 && ((qp->back == qp->size - 1 && qp->front == 0) || qp->back == qp->front - 1))
	{
		printf("Queue full!\n");
		return;
	}

	//Initialize empty queue
	else if(qp->front == -1)
		qp->front = 0;

	//Move back index (circle if needed)
	if(qp->back == qp->size - 1 && qp->front != 0)
		qp->back = 0;
	else
		qp->back = qp->back + 1;

	//Insert item
	qp->innerArray[qp->back] = item;
}

//Removes an item from the front of the queue
void * queue_dequeue(void *q)
{
	Queue *qp = (Queue*)q;
	void *result;
		
	//Queue is empty; exit
	if(qp->front == -1)
	{
		printf("Queue empty! Exiting...\n");
		free(qp);
		exit(1);
	}

	//Get item at front
	result = qp->innerArray[qp->front];

	//Check if queue is empty after this removal
	if(qp->front == qp->back)
		qp->front = qp->back = -1;

	//Move front index (circle if needed)
	else if(qp->front == qp->size - 1)
		qp->front = 0;
	else 
		qp->front = qp->front + 1;

	return result;
}

int queue_isEmpty(void *q)
{
	return (((Queue*)q)->front == -1);
}
