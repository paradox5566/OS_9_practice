/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.14. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.19

Collatz conjecture : n = n/2, if n is even, n = 3*n + 1, if n is odd.

*/


#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <errno.h>

#define LINE_MAX 50

int main(int argc, char *argv)
{
	char input[LINE_MAX];
	int index;
	unsigned long input_buff , input_num;
	pid_t fpid;
/*===========================================================================================
Accept and validate Input.
=============================================================================================*/

	printf("Input Number: ");
	fgets(input, sizeof(input), stdin);

	input_num = 0;

	for(index = 0; index <= strlen(input)-2; index++)
	{
		input_buff = input_num;

		if( input[index] > '9' || input[index] < '0')
		{
			printf("Illegal inputs \n");
			return -1;
		}
		else
		{
			input_num = input_num * 10 + (input[index] - '0');

			if( input_buff > input_num) // only occurs in overflow
			{
				printf("Illegal inputs (overflow)\n");
				return -1;
			}
		}
	}

	fpid = fork();
	
	if(fpid < 0) // fork() error
	{
		perror("fpid = fork()");
		return -1;
	}
	
	else if(fpid == 0) // child process
	{
		index = 0;

		while(input_num > 1)
		{
			if(input_num % 2 == 1) // odd number
			{
				printf("%lu, ", input_num);
				input_num = input_num * 3 + 1;
			}
			else // even number
			{
				printf("%lu, ", input_num);
				input_num = input_num / 2;
				
			}

			if(++index == 8) // start at new line
			{
				index = 0;
				printf("\n");
			}
		}
		printf("%lu.\n", input_num);
		exit(1);
	}

	else // parents process
	{
		printf("Parents Process Excetution \n");
		wait(NULL); 
		printf("Parents Process Complete \n");
	}

	return 0;
}
