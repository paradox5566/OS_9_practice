/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.16. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.21

A quote of day, modified from Fig3.22

*/
 
import java.net.*;
import java.io.*;

public class p16_server
{
	public static void main(String[] args)  {
		try {
			ServerSocket sock = new ServerSocket(6017);

			// now listen for connections
			while (true) {
				Socket client = sock.accept();
				// we have a connection
				
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				// write the Date to the socket
				pout.println("The aim of life is to live, and to live means to be aware, joyously, drunkenly, serenely, divinely aware. -Henry Miller");

				// close the socket and resume listening for more connections
				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
