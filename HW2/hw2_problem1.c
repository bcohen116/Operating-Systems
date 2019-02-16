#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <sys/types.h>//for open/close
#include <sys/stat.h>//for open/close
#include <fcntl.h>//for open/close
#include <sys/wait.h>//for wait funcitons

///<summary>
///What does this do:
/// Take input of program to run, filename to output to
/// Check if number of inputs is correct
/// Create a child using fork
/// In the Child, redirect the stdout to the filename specified in the input
int main(int argc, char* argv[])
{
	int pid,output;

	//check if input is correct
	if (argc != 3) {
		printf("Usage: %s file_name\n", argv[0]);
		exit(1);
	}


	pid = fork();
	if (pid == 0) {
		//This is a child

		//Open or create a file to redirect the output to
		output = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (output < 0) {
			perror("Had trouble creating/opening the file. Error: ");
			exit(1);
		}
		else {
			dup2(output, 1);//redirect the stdout to output file opened above
		}

		//Try to run the system command specified in the input
		execl(argv[1], argv[1], (char*)0);//input of path, commands, ending param


		return 0;//end the child process

	}

	//Dont kill the parent until the child is done
	pid_t finishedPid;
	while (finishedPid > 0) {
		finishedPid = wait(NULL);//wait for children to finish doing everything
		if (finishedPid > 0) {
			printf("Child PID %d Finished running \n \n", finishedPid);
		}
	}
	return 0; //end program
}