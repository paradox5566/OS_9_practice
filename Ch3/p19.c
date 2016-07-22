/*
This is the exercise of <OS Concepts 9/e> programming problems Q3.19. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.22

Using pipe to trasmit the message from one process to the other one. 
After receiving original message, the receiver should reverses the case and return back.

e.g. "Hi There" => "hI tHERE"

In this case, child process sends original message to parents process, 
after reversed by parents process, the message gets sent back.

Modifed from Fig3.25
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	char write_msg[BUFFER_SIZE] = "Hi There";
	char read_msg[BUFFER_SIZE];
	pid_t fpid;
	int original_pipe_fd[2];
	int reverse_pipe_fd[2];

	if (pipe(original_pipe_fd) == -1 || pipe(reverse_pipe_fd) == -1) 
	{
		perror("pipe(original_pipe_fd) == -1 || pipe(reverse_pipe_fd) == -1");
		return -1;
	}

	

	if ((fpid = fork()) < 0) 
	{
		perror("fpid = fork()");
		return -1;
	}

	else if (fpid == 0) // Child process
	{  
		close(original_pipe_fd[READ_END]); // Close unused ends
		close(reverse_pipe_fd[WRITE_END]);

		write(original_pipe_fd[WRITE_END], write_msg, strlen(write_msg)+1); 
		close(original_pipe_fd[WRITE_END]);


/*===========================================================================================
Reads the reverse message from the pipe. (No read, no move.)
=============================================================================================*/
		memset(read_msg, '\0', BUFFER_SIZE);
		memset(write_msg, '\0', BUFFER_SIZE);
		if(read(reverse_pipe_fd[READ_END], read_msg, BUFFER_SIZE) == -1)
		{
			perror("read(original_pipe_fd[READ_END], read_msg, BUFFER_SIZE)");
			return -1;
		}
		close(reverse_pipe_fd[READ_END]);
		printf("child read: %s\n",read_msg);
	}


	else // Parents process
	{ 
		close(original_pipe_fd[WRITE_END]); // Close unused ends
		close(reverse_pipe_fd[READ_END]);

/*===========================================================================================
Reads a message from the pipe and reverse the case of it. 
=============================================================================================*/
		memset(read_msg, '\0', BUFFER_SIZE);
		memset(write_msg, '\0', BUFFER_SIZE);
		if(read(original_pipe_fd[READ_END], read_msg, BUFFER_SIZE) == -1)
		{
			perror("read(original_pipe_fd[READ_END], read_msg, BUFFER_SIZE)");
			return -1;
		}
		close(original_pipe_fd[READ_END]);
		
		int index = 0;

		for(index = 0; index <= BUFFER_SIZE; index++)
		{
			if(read_msg[index] >= 65 && read_msg[index] <= 90)
			{
				write_msg[index] = read_msg[index] + 32;
			}
			else if(read_msg[index] >= 97 && read_msg[index] <= 122)
			{
				write_msg[index] = read_msg[index] - 32;
			}
			else
			{
				write_msg[index] = read_msg[index];
			}
		}
		write(reverse_pipe_fd[WRITE_END], write_msg, strlen(write_msg)+1); 
		close(reverse_pipe_fd[WRITE_END]);
		wait(NULL);

		printf("Parents Process Complete \n");
	}

	return 0;
}


















