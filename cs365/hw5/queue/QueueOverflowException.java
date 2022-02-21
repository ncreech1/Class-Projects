package queue;

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

public class QueueOverflowException extends Exception
{
	private Object item;

	public QueueOverflowException(Object obj)
	{
		item = obj;
	}

	public String getMessage()
	{
		return "queue full: cannot enqueue " + item.toString();
	}
}
