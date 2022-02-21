package race;
import java.util.Queue;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;

/* Nicholas Creech
 * CS365 HW8
 * 04/08/21
 */ 

class TimeKeeper
{
	private Queue<Integer> finishQueue;
	private List<Score> scores;

	public TimeKeeper(int contestants)
	{
		finishQueue = new LinkedList<Integer>();
		scores = new ArrayList<Score>();

		for(int x = 0; x < contestants; x++)
			scores.add(new Score());
	}
	
	//Updates the score object for the contestant with the given ID
	public void reportSegment(int ID, int time)
	{
		ID--; //ID is not zero indexed
		scores.get(ID).totalTime += time;
		scores.get(ID).segment++;
		scores.get(ID).lastTime = time;
		scores.get(ID).newTime = true;
	}

	//Adds a contestant to the finished queue
	public synchronized void reportFinish(int ID)
	{
		finishQueue.add(new Integer(ID));
	}

	//Checks if all contestants are in the finished queue
	public synchronized boolean isFinished()
	{
		return finishQueue.size() == scores.size();
	}

	//Prints the current scoreboard
	public void printScoreboard(int seconds)
	{
		System.out.format("%nResults after %d seconds%n", seconds);
		System.out.println("Contestant  Segments  Time  Last Segment");

		for(int x = 0; x < scores.size(); x++)
		{
			System.out.format("%10d  %8d  %5d  ", x + 1, scores.get(x).segment, scores.get(x).totalTime);
			
			if(scores.get(x).newTime)
				System.out.format("%11d", scores.get(x).lastTime);

			System.out.println("");
			scores.get(x).newTime = false;
		}
	}

	//Prints the final results
	public void printResults()
	{
		System.out.format("%nFinal Results%n");
		
		for(Integer i : finishQueue)
			System.out.println(String.format("%d %d", i.intValue(), scores.get(i.intValue() - 1).totalTime));
	}

	class Score
	{
		public int totalTime, segment, lastTime;
		public boolean newTime;

		public Score()
		{
			totalTime = 0;
			segment = 0;
			newTime = false;
		}
	}
}
