#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <fcntl.h>//for pipe
#include <sys/stat.h>//for pipes
#include <sys/types.h>//for pipes
#include <fstream>//for reading a file line by line
#include <algorithm>//for upper case functions

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
	const char DATA_FILE[] = "hw2_problem4_test_data.txt";//file containing data to send to server
	string line;

	pipe = open(pipeLocation, O_WRONLY);//open the named pipe

	//Open a file that contains the data to send to the server
	ifstream out(DATA_FILE);
	while (getline(out, line)) {
		transform(line.begin(), line.end(), line.begin(), ::toupper);//convert each line to upper case
		write(pipe, line.c_str(), sizeof(line));//actually send the data through the pipe
		printf("Sent Line: %s \n", line.c_str());
		sleep(1);//wait for 1 second to satisfy the problem requirements
	}
	out.close();//safely close the stream
	write(pipe, "STOP", sizeof("STOP"));//tell server we're done
	close(pipe);//safely close the pipe

	return 0;//end program
}