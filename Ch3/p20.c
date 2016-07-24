/*
This is the exercise of <OS Concepts 9/e> programming problems Q3.20. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.23

Using pipe to copy files with 2 input parameters(source file name and destination file name).

The name of executing file should be "filecopy".

e.g. # filecopy input.txt copy.txt

Child process reads source files and parents process copies the content to the dest file.
*/

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define READ_END	0
#define WRITE_END	1

int main(int argc, char *argv[])
{
	char write_buff[BUFFER_SIZE];
	char read_buff[BUFFER_SIZE];
	pid_t fpid;
	int buff_pipe_fd[2];
	int src_fd, dest_fd;
	size_t file_size, read_size, write_size, tmp_size;

	src_fd = dest_fd = file_size = read_size = write_size = tmp_size = 0;

	if ( (src_fd = open(argv[1], O_RDONLY, S_IRWXU)) == -1)
	{
		perror("open(src_path, O_RDONLY, S_IRWXU)");
		return -1;
	}

	file_size = lseek(src_fd, 0, SEEK_END);
	lseek(src_fd, 0, SEEK_SET);

	if (pipe(buff_pipe_fd) == -1) 
	{
		perror("pipe(buff_pipe_fd)");
		close(src_fd);
		return -1;
	}

	if ((fpid = fork()) < 0) 
	{
		perror("fpid = fork()");
		close(src_fd);
		return -1;
	}

	else if (fpid == 0) // Child process
	{  
		close(buff_pipe_fd[READ_END]);

		while(read_size < file_size)
		{
			read_size += read(src_fd, read_buff, BUFFER_SIZE);

			if(write(buff_pipe_fd[WRITE_END], read_buff, BUFFER_SIZE) == -1)
			{
				perror("write(buff_pipe_fd[WRITE_END], read_buff, strlen(read_buff)");
				return -1;
			}

			memset(read_buff, '\0', BUFFER_SIZE);
			lseek(src_fd, read_size, SEEK_SET);
		}
		close(src_fd);
		close(buff_pipe_fd[WRITE_END]);
	}


	else // Parents process
	{ 
		close(buff_pipe_fd[WRITE_END]);

		if ( (dest_fd = open(argv[2], O_WRONLY|O_CREAT, S_IRWXU)) == -1)
		{
			perror("dest_fd = open(argv[2], O_WRONLY|O_CREAT, S_IRWXU)");
			return -1;
		}

		while(write_size < file_size)
		{
			if(read(buff_pipe_fd[READ_END], write_buff, BUFFER_SIZE) == -1)
			{
				perror("read(buff_pipe_fd[READ_END], write_buff, BUFFER_SIZE)");
				close(dest_fd);
				close(buff_pipe_fd[READ_END]);
				return -1;
			}

			if( (tmp_size = write(dest_fd, write_buff, strlen(write_buff))) == -1)
			{
				perror("write(dest_fd, read_buff, BUFFER_SIZE)");
				close(dest_fd);
				close(buff_pipe_fd[READ_END]);
				return -1;
			}
			else
			{
				write_size += tmp_size;
			}
			memset(write_buff, '\0', BUFFER_SIZE);
			lseek(dest_fd, write_size, SEEK_SET);
		}

		close(dest_fd);
		close(buff_pipe_fd[READ_END]);

		wait(NULL);

		printf("Parents Process Complete \n");
	}

	return 0;
}


















