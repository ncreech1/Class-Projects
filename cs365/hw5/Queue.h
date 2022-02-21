#include <string>
#include <sstream>

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

template <typename T>
class Queue
{
	public:
		Queue(int maxCapacity);
		~Queue();
		void enqueue(T value);
		T dequeue();
		void printQueue();
	private:
		T *innerArray;
		int size;
		int front;
		int back;
};

template <typename T>
class QueueOverflowException
{
	public:
		QueueOverflowException(T item) { this->item = item; }
		std::string getMessage() 
		{ 
			std::stringstream ss;
			ss << "queue full: cannot enqueue " << item;
			return ss.str();
		}
	private:
		T item;
};

class QueueEmptyException
{
	public:
		std::string getMessage() { return "queue empty: cannot dequeue"; }
};

#include "Queue.cpp"
