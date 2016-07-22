/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.18. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.22

Echo Server.

*/

import java.net.*;
import java.util.Arrays;
import java.io.*;

public class p18_server
{
	public static void main(String[] args)  
	{
		
		int length = 1024;
				
		try 
		{
			ServerSocket sock = new ServerSocket(5001);
		
			while (true) 
			{
				System.out.println("Server is listening on port 5001...");
				Socket client = sock.accept();
				
				int sock_status = 0;
				byte[] input_Buff = new byte[length];
				InputStream is = client.getInputStream();
				OutputStream os = client.getOutputStream();
				
				PrintWriter pout = new PrintWriter(os, true);
				pout.println("HELO, this is an echo server.");
				
				while(sock_status != -1)
				{
					sock_status = is.read(input_Buff);
					String echo_Line = new String(input_Buff, "UTF-8");
					pout.println("Reply:" + echo_Line);
				}
				is.close();
				os.close();
				client.close();
			}
		}
		
		catch (IOException ioe) 
		{
				System.err.println(ioe);
		}
	}
}










 
