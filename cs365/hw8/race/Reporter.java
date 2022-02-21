package race;

/* Nicholas Creech
 * CS365 Hw8
 * 04/08/21
 */ 

class Reporter implements Runnable
{
	private TimeKeeper timeKeeper;

	public Reporter(TimeKeeper timeKeeper)
	{
		this.timeKeeper = timeKeeper;
	}

	@Override
	public void run()
	{
		int seconds = 2;

		try
		{
			System.out.println("Start the race!");

			//Print scoreboard every two seconds
			while(!timeKeeper.isFinished())
			{
				Thread.sleep(2000);
				timeKeeper.printScoreboard(seconds);

				seconds += 2;
			}
			
			//Race finished; print results
			timeKeeper.printResults();
		}

		catch(InterruptedException e)
		{
			System.out.println("Unexpected interrupt in reporter thread");
		}
	}
}
