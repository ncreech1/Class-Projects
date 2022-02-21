package race;

/* Nicholas Creech
 * CS365 HW8
 * 04/08/21
 */ 

public class Race
{
	public Race(String args[])
	{
		TimeKeeper timeKeeper;
		SynchronizedRandom rand;
		Reporter report;
		int contestants;

		contestants = Integer.parseInt(args[0]);
		timeKeeper = new TimeKeeper(contestants);
		rand = new SynchronizedRandom();

		//Start contestant threads
		for(int x = 1; x <= contestants; x++)
			(new Thread(new Contestant(x, rand, timeKeeper))).start();
	
		//Start reporter thread
		(new Thread(new Reporter(timeKeeper))).start();
	}

	public static void main(String args[])
	{
		if(args.length != 1)
		{
			System.out.println("Usage: java -jar race.jar <contestants>");
			return;
		}

		new Race(args);
	}
}
