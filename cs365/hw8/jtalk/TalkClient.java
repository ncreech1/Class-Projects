package jtalk;
import java.net.Socket;
import java.net.UnknownHostException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;

/* Nicholas Creech
 * CS365 HW8
 * 04/15/21
 */ 

public class TalkClient
{
	public TalkClient(String args[])
	{
		int port;
		String usage;
		String fromUser;
		Socket serverSocket;
		ClientThread printThread;

		//Get port number from command line args
		usage = "Usage: java -jar TalkClient.jar <hostname> <port> <chatname>";
		try { port = Integer.parseInt(args[1]); }
		catch(Exception e) { System.out.println(usage); return; }
	
		try 
		{ 
			//Connect to server
			serverSocket = new Socket(args[0], port); 
			PrintWriter out = new PrintWriter(serverSocket.getOutputStream(), true);
			BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
			BufferedReader in = new BufferedReader(new InputStreamReader(serverSocket.getInputStream()));

			//Send name to server
			out.println(args[2]);

			//Start thread that listens to server output stream
			printThread = (new ClientThread(in));
			printThread.start();

			//Continuously send stdin from user to server
			while((fromUser = stdin.readLine()) != null)
				out.println(fromUser);
			
			//Send interrupt to second client thread
			printThread.interrupt = true;
	
			out.close();
			in.close();
			stdin.close();
			serverSocket.close();
		}

		catch(UnknownHostException e) { System.out.println("Unknown host '" + args[0] + "'"); }
		catch(IOException e) { System.out.println("Exception establishing connection to server"); }
	}

	public static void main(String args[])
	{
		new TalkClient(args);	
	}
}
