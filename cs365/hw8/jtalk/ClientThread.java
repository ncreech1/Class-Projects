package jtalk;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

/* Nicholas Creech
 * CS365 HW8
 * 04/15/21
 */ 

class ClientThread extends Thread
{
	public boolean interrupt;
	private BufferedReader in;

	public ClientThread(BufferedReader in)
	{
		this.in = in;
		interrupt = false;
	}

	@Override
	public void run()
	{
		String fromServer;

		try
		{
			//Continuously print server output to stdout
			while((fromServer = in.readLine()) != null)
				System.out.println(fromServer);
		}

		catch(IOException e) 
		{
			//Interrupt is not an exception
			if(!interrupt)
				System.out.println("ClientThread.java: Exception reading server output"); 
		}
	}
}
