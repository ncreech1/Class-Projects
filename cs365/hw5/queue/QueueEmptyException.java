package queue;

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

public class QueueEmptyException extends Exception
{
	public String getMessage()
	{
		return "queue empty: cannot dequeue";
	}
}
