/*
This is the exercise of <OS Concepts 9/e> programming project #1 in Chapter3. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.27

UNIX shell.

(1)Creating the child process and executing the command in the child.
(2)Modifying the shell to allow a history feature.

PS. "quit" to EXIT.
*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_PARAMS 20
#define MAX_LINE 80
#define HISTORY_BUFF 100
int main(void)
{
	char *args[MAX_PARAMS];
	char line_buff[MAX_LINE];
	char log_buff[HISTORY_BUFF][MAX_LINE];
	char *token;
	int wrap_around = 0;
	pid_t fpid;
	int index;
	int log_index = 0;

	while(1)
	{
/*===========================================================================================
Initialization : promt and accept inputs
=============================================================================================*/
		memset(line_buff, '\0', MAX_LINE);
		printf("osh>");
		fflush(stdout);
		fgets(line_buff, sizeof(line_buff), stdin);
		
		if(strcmp(line_buff, "quit\n") == 0)
		{
			printf(">>>simple_shell exit \n");
			return 0;
		}
		else if(strcmp(line_buff, "history\n") == 0)
		{
			int i;

			if(wrap_around == 1)
			{
				for(i = 0; i < 10; i++)
				{
					printf("#%d: %s \n", (log_index - i + 100) % 100, log_buff[(log_index - i + 100) % 100]);
				}
			}
			else if(wrap_around == 0)
			{
				for(i = 0; i < log_index; i++)
				{
					printf("#%d: %s \n", log_index - i, log_buff[log_index - i - 1]);
				}
			}
			else
			{
				perror("wrap_around error!");
				return -1;	
			}
		}
		else
		{
/*===========================================================================================
* Separate the input by space. 
* 
* strtok() function takes two times execution, first time takes original string and separator 
* which will return the pointer to the first separate result and then the next time execution 
* will be the process of walking through the rest of the string.
=============================================================================================*/
			line_buff[strlen(line_buff) - 1] = '\0';
			memset(log_buff[log_index], '\0', MAX_LINE);
			strcpy(log_buff[log_index++], line_buff);

			if(log_index == 100 && wrap_around == 0)
			{
				wrap_around = 1;
				log_index = 0;
			}
			else if(log_index == 10 && wrap_around == 1)
			{
				wrap_around = 0;
			}

			token = strtok(line_buff, " ");
			index = 0;


			while(token != NULL)
			{
				args[index++] = token;
				token = strtok(NULL, " ");
			}

			if ((fpid = fork()) < 0) 
			{
				perror("fpid = fork()");
				return -1;
			}
			else if (fpid == 0) // Child process
			{
				if(strcmp(args[index-1], "&") == 0)
				{
					args[index-1] == '\0';
				}

				else if(execvp(args[0], args) == -1)
				{
					perror("execvp(args[0], args)");
					return -1;
				}
				exit(1);				
			}
			else // Parents process
			{ 
				if(strcmp(args[index-1], "&") != 0)
				{
					wait(NULL);
				}
				printf("Parents Process Complete \n");
			}
		
		}
				
	}
	

	return 0;
}


















