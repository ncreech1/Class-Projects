#include <iostream>
#include <cstdlib>

using namespace std;

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

template <typename T>
Queue<T>::Queue(int maxCapacity)
{
	innerArray = new T[maxCapacity];
	size = maxCapacity;
	front = -1;
	back = -1;
}

template <typename T>
Queue<T>::~Queue()
{
	delete[] innerArray;
}

template <typename T>
void Queue<T>::enqueue(T value)
{
	//Queue is full
	if(front != -1 && ((back == size - 1 && front == 0) || (back == front - 1)))
		throw QueueOverflowException<T>(value);

	//Initialize empty queue
	else if(front == -1)
		front = 0;

	//Move back index (circle if needed)
	if(back == size - 1 && front != 0)
		back = 0;
	else
		back++;

	innerArray[back] = value;
}

template <typename T>
T Queue<T>::dequeue()
{
	T result;

	//Queue is empty
	if(front == -1)
		throw QueueEmptyException();

	//Get item at front
	result = innerArray[front];

	//Check if queue is empty after this removal
	if(front == back)
		front = back = -1;

	//Move front index (circle if needed)
	else if(front == size - 1)
		front = 0;
	else
		front++;

	return result;
}

template <typename T>
void Queue<T>::printQueue()
{
	if(front == -1)
		return;

	for(int x = front; x != back; x++)
	{
		if(x == size)
			x = 0;
		if(x == back)
			break;

		cout << innerArray[x] << endl;
	}

	cout << innerArray[back] << endl;
}
