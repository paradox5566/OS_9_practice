/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.16. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.21

A quote of day, modified from Fig3.22

*/

import java.net.*;
import java.io.*;

public class p16_client
{
	public static void main(String[] args)  {
		try {
			// this could be changed to an IP name or address other than the localhost
			Socket sock = new Socket("127.0.0.1",6017);
			InputStream in = sock.getInputStream();
			BufferedReader bin = new BufferedReader(new InputStreamReader(in));

			String line;
			while( (line = bin.readLine()) != null)
				System.out.println(line);
				
			sock.close();
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
