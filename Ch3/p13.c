/*
 
This is the exercise of <OS Concepts 9/e> programming problems Q3.13. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.19

PID Manager

*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h> 
#define MIN_PID 300
#define MAX_PID 5000

const int pool_size = MAX_PID-MIN_PID+1;

bool pid_pool[MAX_PID-MIN_PID+1]; // cannot use pool_size which causes "variably modified at file scope"

int main(int argc, char *argv)
{
	return 0;
}


/*===========================================================================================
Creates and initializes a data structure for representing pids; return -1 if unsuccessful, 
1 if successful.
=============================================================================================*/
int allocate_map(void)
{
	int index;
	for(index = 0; index <= pool_size; index++)
	{
		pid_pool[index] = true;
	}
	if(index <= pool_size)
		return 1;
	else
		return -1;
}


/*===========================================================================================
Allocates and returns a pid; returns -1 if unable to allocate a pid (all pids are in use)
=============================================================================================*/
int allocate_pid(void)
{
	int index, pid;

	index = 0;
	pid = -1;

	while(index <= pool_size && pid < 0)
	{
		if(pid_pool[index] == true)
		{
			pid = index + MIN_PID;
			pid_pool[index++] = false;
		}
	}
	if (index > pool_size && pid < 0) // traverse the map and cannot find an available pid
		return -1;
	else
		return pid;
}


/*===========================================================================================
Releases a pid.
=============================================================================================*/
void release_pid(int pid)
{
	if (pid_pool[pid-MIN_PID] == true)
		printf(">>> pid %d is already free\n", pid);
	else
		pid_pool[pid-MIN_PID] == true;
}

