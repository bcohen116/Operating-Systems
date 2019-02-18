#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <cstring>//for strncpy
#include <sys/mman.h>
#include <fstream>//for reading a file line by line
#include <algorithm>//for upper case functions
#include <fcntl.h>//for open permissions

using namespace std;

///NOTE: Run the server before the client

///<summary>
///What does this do:
/// USING  mmap shared memory instead of SysV:
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
	const char FILE_NAME[] = "hw2_problem4_SysV_memory.dat";//mmap shared memory file
	const char DATA_FILE[] = "hw2_problem4_test_data.txt";//file containing data to send to server

	key_t key;
	int n, dataFile, sharedFile;
	char * data;
	string line;

	sharedFile = open(FILE_NAME, O_RDWR, 0600);//open shared memory file
	if (sharedFile < 0) {
		perror("failed to open file to use for shared memory, error: ");
		exit(1);
	}

	//we have to write one extra byte to ensure the map is the right size
	if (write(sharedFile, "", 1) == -1)
	{
		close(sharedFile);
		perror("Error writing last byte of the file");
		exit(1);
	}

	char * shared_memory = (char*) mmap(0, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedFile, 0);
	if (shared_memory == (caddr_t) -1) {
		perror("mmap error: ");
		exit(1);
	}

	//Open a file that contains the data to send to the server
	ifstream out(DATA_FILE);
	while (getline(out, line)) {
		transform(line.begin(), line.end(), line.begin(), ::toupper);//convert each line to upper case
		// Write it now to disk
		strncpy(shared_memory, line.c_str(), MEMORY_SIZE);//send the line to the shared memory so server can get it
		//tell the map to sync so it actually writes to disk
		if (msync((void*) shared_memory, MEMORY_SIZE, MS_SYNC) < 0)
		{
			perror("Could not sync the file to disk");
			exit(1);
		}
		printf("Line sent: %s \n", line.c_str());
		sleep(1);//wait for 1 second to satisfy the problem requirements
	}
	out.close();//safely close the stream
	strncpy(shared_memory, "STOP", MEMORY_SIZE);//tell server we're done

	//Done with work, cleanup
	close(sharedFile);

	return 0;//end client
}