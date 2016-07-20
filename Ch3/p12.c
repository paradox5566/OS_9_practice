/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.12. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.19

Create a zombie process and make it remains in the OS for more than 10 seconds.

Hints: Using "#ps -al" in Ubuntu 14.04 instead of "ps -l" which illustrated in the book.

*/


#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv)
{
	pid_t fpid; // stands for the return value (pid) of function fork()
	int status; // store child process return value (it can be either explicitly return value or implicitly exit(value))

	fpid = fork();
	
	if(fpid < 0) // fork() error
	{
		perror("fpid = fork()");
		return -1;
	}
	
	else if(fpid == 0) // child process
	{
		printf("Child Process Excetution \n");
	}

	else // parents process
	{
		printf("Parents Process Excetution \n");
		sleep(30);
		wait(&status); // Either wait or not, when child process exits, the parents process does not wait cause child become zombie
		printf("Parents Process Complete \n");
	}

	return 0;
}
