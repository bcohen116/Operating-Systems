#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <cstring>//for strncpy
#include <sys/shm.h>//semaphores
#include <sys/ipc.h>//semaphore stuff
#include <fstream>//for reading a file line by line
#include <algorithm>//for upper case functions

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
	const char FILE_NAME[] = "hw2_problem4_SysV_memory.dat";//semaphore shared memory file
	const char DATA_FILE[] = "hw2_problem4_test_data.txt";//file containing data to send to server

	key_t key;
	int shmId, n,dataFile;
	char * data;
	string line;

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
	data = (char*) shmat(shmId, (void*)0, 0);
	if (data == (char *)(-1)) {
		perror("shmat failed, error: ");
		exit(1);
	}

	//Open a file that contains the data to send to the server
	ifstream out(DATA_FILE);
	while (getline(out, line)) {
		transform(line.begin(), line.end(), line.begin(), ::toupper);//convert each line to upper case
		strncpy(data, line.c_str(), MEMORY_SIZE);//send the line to the shared memory so server can get it
		printf("Line sent: %s \n", line.c_str());
		sleep(1);//wait for 1 second to satisfy the problem requirements
	}
	out.close();//safely close the stream
	strncpy(data, "STOP", MEMORY_SIZE);//tell server we're done

	//Done with work, detach the segment
	if (shmdt(data) == -1) {
		perror("Failed to detatch, error: ");
		exit(1);
	}

	return 0;//end client
}