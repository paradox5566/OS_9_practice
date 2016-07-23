/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.15. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.19

Collatz conjecture + Shared Memory

>>> Compile: gcc (source file) -o (output file name) -lrt  
>>> -l means linked to search related library, the parameters follow it is the name of library
>>> "rt" stands for "Realtime Extensions Library"

*/


#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>

#define MEM_SIZE 1024*1024
#define LINE_MAX 1024

int main(int argc, char *argv[])
{

	pid_t fpid;
	int offset,index;
	long mem_size;

	char buff[LINE_MAX];
	unsigned long input_buff, input_num;

/*===========================================================================================
Accept and validate Input.
=============================================================================================*/

	printf("Input Number: ");
	fgets(buff, sizeof(buff), stdin);

	input_num = 0;

	for(index = 0; index <= strlen(buff)-2; index++)
	{
		input_buff = input_num;

		if( buff[index] > '9' || buff[index] < '0')
		{
			printf("Illegal inputs \n");
			return -1;
		}
		else
		{
			input_num = input_num * 10 + (buff[index] - '0');

			if( input_buff > input_num) // only occurs in overflow
			{
				printf("Illegal inputs (overflow)\n");
				return -1;
			}
		}
	}
/*===========================================================================================
a. Establish the shared-memory object
=============================================================================================*/
	int fd, ret;

	fd = shm_open("ch3p15", O_CREAT|O_RDWR|O_TRUNC, 0666); // Similar to creating file POSIX

	if (fd == -1)
	{
		perror("shm_open(\"ch3p15\", O_CREAT|O_RDWR|O_TRUNC, 0666)");
		return -1;
	}

	ret = ftruncate(fd, MEM_SIZE); // Resize the file

	if (ret == -1)
	{
		perror("ftruncate(fd, MEM_SIZE)");
		close(fd);
		return -1;
	}

	void *map_status = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(map_status == (void*)(-1))
	{
		perror("mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)");
		close(fd);
		return -1;
	}

/*===========================================================================================
b. Create the child process and wait for it to terminate
=============================================================================================*/	
	fpid = fork();
	
	if(fpid < 0) // fork() error
	{
		perror("fpid = fork()");
		return -1;
	}
	
	else if(fpid == 0) // child process
	{
		index = offset = 0;
		memset(buff, '\0', LINE_MAX); // clear the buffer
		lseek(fd, 0, SEEK_SET);

		while(input_num > 1)
		{
			if(input_num % 2 == 1) // odd number
			{
				offset = offset + sprintf( buff + offset, "%lu, ", input_num);
				input_num = input_num * 3 + 1;
			}
			else // even number
			{
				offset = offset + sprintf( buff + offset, "%lu, ", input_num);
				input_num = input_num / 2;
			}

			if(++index == 8) // start at new line
			{
				index = 0;
				offset = offset + sprintf( buff + offset, "\n");
			}
			
			if(offset > 1000) // write to memory if buffer is full
			{
				if(write(fd, buff, strlen(buff)) == -1)
				{
					perror("write(fd, buff, strlen(buff))");
					close(fd);
					return -1;
				}
				else
				{
					lseek(fd, strlen(buff), SEEK_SET);
					memset(buff, '\0', LINE_MAX);
					offset = 0;
				}
			}
		}
		offset = offset + sprintf( buff + offset, "%lu.\n", input_num);

		if(write(fd, buff, strlen(buff)) == -1)
		{
			perror("write(fd, buff, strlen(buff))");
			close(fd);
			return -1;
		}
		else
		{
			lseek(fd, strlen(buff), SEEK_SET);
		}
		printf("Child Process Complete \n");
		exit(1);
	}
/*===========================================================================================
c. Output the content of shared memory
=============================================================================================*/	
	else // parents process
	{
		printf("Parents Process Excetution \n");
		wait(NULL); 
		mem_size = lseek(fd, 0, SEEK_END);
		lseek(fd, 0, SEEK_SET);
		offset = 0;

		memset(buff, '\0', LINE_MAX);

		while(offset < mem_size)
		{
			if(read(fd, buff, LINE_MAX) == -1)
			{
				perror("read(fd, buff, LINE_MAX)");
				close(fd);
				return -1;
			}
			else
			{
				printf("%s", buff);
				offset = offset + LINE_MAX;
				memset(buff, '\0', LINE_MAX);
			}
		}
		
/*===========================================================================================
d. Remove the shared-memory object
=============================================================================================*/	
		if(munmap(map_status, MEM_SIZE) == -1)
		{
			perror("munmap(ptr, MEM_SIZE)");
			return -1;
		}
		if (close(fd) == -1)
		{
			perror("close(fd)");
			return -1;
		}
		printf("Parents Process Complete \n");
	}
	

	return 0;
}




































