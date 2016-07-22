/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.17. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.21

Haiku: first line with 5 syllables, second line with 7 syllables, third line with 5 syllables.

*/
 
import java.net.*;
import java.io.*;

public class p17_server
{
	public static void main(String[] args)  {
		try {
			ServerSocket sock = new ServerSocket(5575);

			while (true) {
				Socket client = sock.accept();
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				pout.println(" ha-tsu shi-gu-re \n sa-ru mo ko-mi-no wo \n ho-shi-ge na-ri");

				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
