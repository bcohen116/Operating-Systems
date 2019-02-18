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

	fclose(fopen(FILE_NAME, "w"));
	sharedFile = open(FILE_NAME, O_RDWR, 0600);//open shared memory file, have to use RDWR or mmap will fail
	if (sharedFile < 0) {
		perror("failed to open file to use for shared memory, error: ");
		exit(1);
	}
	write(sharedFile, "empty", sizeof("empty"));//doing this to reset the contents for mmap later

	//create shared memory map
	char * shared_memory = (char*) mmap(0, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedFile, 0);
	if (shared_memory == (caddr_t)-1) {
		perror("mmap error: ");
		exit(1);
	}

	//read contents of shared memory until we're done
	ifstream out(DATA_FILE);
	while (getline(out, line)) {
		printf("Line Received: %s \n", line.c_str());
	}
	out.close();//safely close the stream

	//Done with work, server cleanup
	if (munmap(shared_memory, MEMORY_SIZE) < 0) {
		close(sharedFile);
		perror("Could not unmap the shared memory file, error: ");
		exit(1);
	}
	close(sharedFile);

	return 0;//end client
}