#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <cstring>//for strncmp
#include <fcntl.h>//for pipe
#include <sys/stat.h>//for pipes
#include <sys/types.h>//for pipes

using namespace std;

///NOTE: Run the server before the client

///<summary>
///What does this do:
///   Create a server and a client
///   client reads from text file using named pipe
///   convert everything to upper case, send data to server using named pipe
///   client waits for 1 second then repeats
///   client sends "Stop" at the end of the data
///   Server displays anything received
///   Server cleans up after receiving "Stop" and removes the pipe
///</summary>
int main(int argc, char* argv[]) {
	int pipe;
	const int MAX_STORAGE = 1024;//max buffer size
	const char * pipeLocation = "/tmp/myfifopipe";
	char buf[MAX_STORAGE];

	mkfifo(pipeLocation, 0666);//create a FIFO pipe

	pipe = open(pipeLocation, O_RDONLY);//create a pipe
	while (strcmp(buf, "STOP") != 0) {
		read(pipe, buf, MAX_STORAGE);//read contents of pipe
		if (strcmp(buf, "STOP") == 0) {
			//extra check to make sure we dont print out the "stop" line
			break;
		}
		printf("Received from pipe: %s \n", buf);
	}
	
	close(pipe);//safely close the pipe

	
	unlink(pipeLocation);//remove the FIFO pipe

	return 0;
}