/*

This is the exercise of <OS Concepts 9/e> programming problems Q2.15. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.18

In Section 2.3, we described a program that copies the contents of one file to a destination file. This program works by first prompting the user 
for the name of the source and destilcation files. Write this program using either the Win32 or POSIX API. Be sure to include all necessary error 
checking, including ensuring that the source file exists. 

*/



#include <fcntl.h> // open()
#include <unistd.h> // close()

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define LINE_MAX 50
#define BLOCK_SIZE 1024
int main(int argc, char *argv)
{
	char src_path[LINE_MAX], dest_path[LINE_MAX];
	int src_fd = -1;
	int dest_fd = -1;


/*===========================================================================================
Acquire Input, Prompt, Accept Input and check validate or not.
=============================================================================================*/
	printf("Please type in the source file path (absolute path is preferred): \n");
	printf("Source file path: ");
	fgets(src_path, sizeof(src_path), stdin);

	if( src_path[strlen(src_path)-1] == '\n' )
		src_path[strlen(src_path)-1] = '\0';

	printf("Please type in the destination file path (absolute path is preferred): \n");
	printf("Destination file path: ");
	fgets(dest_path, sizeof(dest_path), stdin);

	if( dest_path[strlen(dest_path)-1] == '\n' )
		dest_path[strlen(dest_path)-1] = '\0';

	src_fd = open(src_path, O_RDONLY);

	if (src_fd == -1)
	{
		perror("open(src_path, O_RDONLY)");
		return -1;
	}
	
	if (close(src_fd) == -1)
	{
		perror("close(src_fd)");
		return -1;
	}

	dest_fd = open(dest_path, O_CREAT|O_EXCL, S_IRWXU);

	if (dest_fd == -1)
	{
		perror("open(dest_path, O_CREAT|O_EXCL)");
		return -1;
	}
	
	if (close(dest_fd) == -1)
	{
		perror("close(dest_fd)");
		return -1;
	}
	
/*===========================================================================================
Loop read, write operations
=============================================================================================*/
	src_fd = open(src_path, O_RDONLY);
	
	char *buff = malloc(BLOCK_SIZE);
	size_t read_size = 0;
	size_t write_size = 0;
	size_t file_size = lseek(src_fd, 0, SEEK_END);

	lseek(src_fd, 0, SEEK_SET);
	
	while(write_size < file_size)
	{
		memset(buff, '\0', BLOCK_SIZE); // clear the buffer
		read_size = read(src_fd, buff, BLOCK_SIZE);
	
		if (close(src_fd) == -1)
		{
			perror("close(src_fd)");
			return -1;
		}

		dest_fd = open(dest_path, O_WRONLY|O_APPEND, S_IRWXU);
		if (dest_fd == -1)
		{
			perror("open(dest_path, O_CREAT|O_EXCL)");
			return -1;
		}
	
		if(write(dest_fd, buff, strlen(buff)) == -1)
		{
			perror("write(dest_fd, buff, BLOCK_SIZE)");
			return -1;
		}
		write_size = write_size + strlen(buff);
		printf(">>>Copy %lu/%lu \n", write_size, file_size);
	
		if (close(dest_fd) == -1)
		{
			perror("close(dest_fd)");
			return -1;
		}

		src_fd = open(src_path, O_RDONLY);
		if (src_fd == -1)
		{
			perror("open(src_path, O_RDONLY)");
			return -1;
		}
		lseek(src_fd, write_size, SEEK_SET);
	}

	if (close(src_fd) == -1)
	{
		perror("close(src_fd)");
		return -1;
	}

	free(buff);
	
	return 0;
}

