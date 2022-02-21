package race;

/* Nicholas Creech
 * CS365 HW8
 * 04/08/21
 */ 

class Contestant implements Runnable
{
	private SynchronizedRandom rand;
	private TimeKeeper timeKeeper;
	private int ID;

	public Contestant(int ID, SynchronizedRandom rand, TimeKeeper timeKeeper)
	{
		this.ID = ID;
		this.rand = rand;
		this.timeKeeper = timeKeeper;
	}
	
	@Override
	public void run()
	{
		try
		{
			//Run four segments
			for(int x = 0; x < 4; x++)
			{
				int segmentTime = rand.nextInt();
				Thread.sleep(segmentTime);

				//Report the last segment
				timeKeeper.reportSegment(ID, segmentTime);
			}
			
			//Report that this contestant has finished
			timeKeeper.reportFinish(ID);
		}

		catch(InterruptedException e)
		{
			System.out.println("Unexpected interrupt for contestant " + Integer.toString(ID));
		}
	}
}
