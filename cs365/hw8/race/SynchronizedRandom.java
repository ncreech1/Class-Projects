package race;
import java.util.Random;

/* Nicholas Creech
 * CS365 HW8
 * 04/08/21
 */ 

class SynchronizedRandom
{
	private Random rand;

	public SynchronizedRandom()
	{
		rand = new Random();
	}

	//Returns an int between 1000 and 8000
	public synchronized int nextInt()
	{
		return rand.nextInt(7001) + 1000;
	}
}
