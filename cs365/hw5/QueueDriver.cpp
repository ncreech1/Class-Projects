#include "Queue.h"
#include <iostream>

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

int main()
{
	string word;
	int size;
	Queue<string> *q;

	//Instantiate queue
	cin >> size;
	q = new Queue<string>(size);

	//Get commands from stdin
	while(cin >> word)
	{
		try
		{
			if(word == "enqueue")
			{
				cin >> word;
				q->enqueue(word);
			}

			else if(word == "dequeue")
				cout << "dequeue: " << q->dequeue() << endl;

			else if(word == "print")
			{
				cout << "queue contents" << endl;
				q->printQueue();
			}

			else
				cout << "invalid command: " << word << endl;
		}

		catch(QueueOverflowException<string> qe) { cout << qe.getMessage() << endl; }
		catch(QueueEmptyException qe) { cout << qe.getMessage() << endl; }
	}

	return 0;
}
