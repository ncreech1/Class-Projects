package queue;

import java.util.ArrayList;

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

public class Queue<T>
{
	private ArrayList<T> innerArray;
	private int front;
	private int back;

	public Queue(int maxCapacity)
	{
		innerArray = new ArrayList<T>(maxCapacity);
		front = -1;
		back = -1;

		//Initialize size
		for(int x = 0; x < maxCapacity; x++)
			innerArray.add(null);
	}

	public void enqueue(T value) throws QueueOverflowException
	{
		//Queue is full
		if(front != -1 && ((back == innerArray.size() - 1 && front == 0) || (back == front - 1)))
			throw new QueueOverflowException(value);

		//Initialize empty queue
		else if(front == -1)
			front = 0;
		
		//Move back index (circle if needed)
		if(back == innerArray.size() - 1 && front != 0)
			back = 0;
		else
			back++;
	
		//Insert item
		innerArray.set(back, value);
	}

	public T dequeue() throws QueueEmptyException
	{
		T result;

		//Queue is empty
		if(front == -1)
			throw new QueueEmptyException();

		//Get item at front of queue
		result = innerArray.get(front);

		//Check if queue is empty after this removal
		if(front == back)
			front = back = -1;

		//Move front index (circle if needed)
		else if(front == innerArray.size() - 1)
			front = 0;
		else
			front++;

		return result;
	}

	public void printQueue()
	{
		if(front == -1)
			return;

		for(int x = front; x != back; x++)
		{
			if(x == innerArray.size())
				x = 0;
			if(x == back)
				break;

			System.out.println(innerArray.get(x));
		}

		System.out.println(innerArray.get(back));
	}
}
