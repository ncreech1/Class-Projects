package queue;

import java.util.Scanner;

/* Nicholas Creech
 * CS365 HW5
 * 03/05/21
 */ 

public class QueueDriver
{
	public QueueDriver()
	{
		Queue<String> q;
		Scanner sc, t;
		String word;

		//Create scanner for stdin and get queue size
		sc = new Scanner(System.in);
		t = new Scanner(sc.nextLine());
		q = new Queue<String>(t.nextInt());
		t.close();

		//Read in commands
		while(sc.hasNextLine())
		{
			//Create tokenizer scanner
			t = new Scanner(sc.nextLine());
			word = t.next();

			try
			{
				if(word.equals("enqueue"))
				{
					word = t.next();
					q.enqueue(word);
				}

				else if(word.equals("dequeue"))
					System.out.println("dequeue: " + q.dequeue());
				
				else if(word.equals("print"))
				{
					System.out.println("queue contents");
					q.printQueue();
				}

				else
					System.out.println("invalid command: " + word);
			}

			catch(QueueOverflowException qe) { System.out.println(qe.getMessage()); }
			catch(QueueEmptyException qe) { System.out.println(qe.getMessage()); }
			finally { t.close(); }
		}

		sc.close();
	}

	public static void main(String args[])
	{
		new QueueDriver();
	}
}
