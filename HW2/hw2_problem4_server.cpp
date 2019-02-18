#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <cstring>//for strncmp
#include <sys/shm.h>//semaphores
#include <sys/ipc.h>//semaphore stuff

using namespace std;

///NOTE: Run the server before the client

///<summary>
///What does this do:
/// USING  SysV shared memory instead of problem 3:
///   Create a server and a client
///   client reads from text file
///   convert everything to upper case, send data to server using named pipe
///   client waits for 1 second then repeats
///   client sends "Stop" at the end of the data
///   Server displays anything received
///   Server cleans up after receiving "Stop"
///</summary>
int main(int argc, char* argv[]) {
	const int MEMORY_SIZE = 1024; //1k memory to share between client and server
	const char FILE_NAME[] = "hw2_problem4_SysV_memory.dat";
	key_t key;
	int shmId,n;
	char * data;

	//Create a key to link the two programs
	if ((key = ftok(FILE_NAME, 1)) == -1) {
		perror("ftok failed, error: ");
		exit(1);
	}

	//Try to connect to a segment or make a new one
	if ((shmId = shmget(key, MEMORY_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget failed, error: ");
		exit(1);
	}

	//Attach to the segment from above
	data = (char*)shmat(shmId, (void*)0, 0);
	if (data == (char *)(-1)) {
		perror("shmat failed, error: ");
		exit(1);
	}

	//read the contents of the segment
	n = 0;
	while (strcmp(data,"STOP") != 0) {
		//Check to make sure the data is new
		if (*data != (n - 1)) {
			printf("Current segment: %s \n", data);
			*data = n;//change the first char of the segment to n so we know that we have read this segment
			n++;
		}
	}

	//Done with work, detach the segment
	if (shmdt(data) == -1) {
		perror("Failed to detatch, error: ");
		exit(1);
	}

	//Safely close the semaphore
	shmctl(shmId, IPC_RMID, NULL);

	return 0;//end program
}