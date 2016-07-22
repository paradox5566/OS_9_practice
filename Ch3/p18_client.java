/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.18. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.22

Echo client.

*/

import java.net.*;
import java.io.*;

public class p18_client
{

	
	public static void main(String[] args)  
	{
		try
		{
			p18_client program = new p18_client();
			program.run(args);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void run(String[] args)
	{
		byte[] out = new byte[1024];
		int sock_Status = 0;
		String line = "";
		Socket sock = null;
		InputStream is = null;
		OutputStream os = null;
		
		try
		{
			sock = new Socket("127.0.0.1",5001);
			is = sock.getInputStream();
			 os = sock.getOutputStream();
		}
		catch(IOException ioe)
		{
			ioe.printStackTrace();
		}
		BufferedReader sock_buffer = new BufferedReader(new InputStreamReader(is));
		BufferedReader stdin_buffer = new BufferedReader(new InputStreamReader(System.in));
				
		this.wait_Response(line, sock_buffer);
		
		
		if(line.equals("quit") == false)
		{
			try
			{
				line = stdin_buffer.readLine();
				out = line.getBytes("UTF-8");
				os.write(out);
				os.flush();
			}
			catch(IOException ioe)
			{
				System.err.println(ioe);
			}
								
			this.wait_Response(line, sock_buffer);
		}
		else
		{
			this.close_Socket(is, os, sock);
		}

	}
			
	public void wait_Response(String line, BufferedReader sock_buffer)
	{
		try
		{
			if( (line = sock_buffer.readLine()) != null)
			{
				System.out.println(line);
			}
			else
			{
				System.out.println("Server no response");
			}
		}
		catch (IOException ioe) 
		{
				System.err.println(ioe);
		}
	}
	
	public void close_Socket(InputStream is, OutputStream os, Socket sock)
	{
		try
		{
			os.close();
			is.close();
			sock.close();
		}
		catch(IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}
