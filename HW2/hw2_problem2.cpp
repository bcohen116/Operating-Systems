#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>
#include <cstring>//for strncmp
#include <fcntl.h>//for pipe
#include <sys/stat.h>//for pipes
#include <sys/types.h>//for pipes

using namespace std;

///<summary>
///What does this do:
/// Using 2 pipes and popen
/// Find all file names starting in this directory that are c files
/// Use recursion to find file names in subdirectories
/// Display the result in all upper case using another popen
///</summary>
int main(int argc, char* argv[]) {
	char buffer[256];
	string resultList;
	string finalResultList;
	FILE * upcaseList;
	
	//Technically all we need is this one line, but the problem asks for two pipes
	//FILE * fileList = popen("find . -name \\*.c -print | tr a-z A-Z", "r");//runs unix command to find all files ending in .c in this directory

	//Since we need two pipes, we will split this up into getting the list, and then upcasing the list
	//First pipe
	FILE * fileList = popen("find . -name \\*.c -print", "r");//runs unix command to find all files ending in .c in this directory
	//read the contents of popen into a buffer, then save to a string
	while (fgets(buffer, sizeof buffer, fileList) != NULL) {
		resultList += buffer;
	}
	
	//At this point we have the list of files, now we need to convert them using the second pipe
	string commandString = "echo \"";//escaping a quote so we can ignore line breaks in resultList
	commandString += resultList.c_str();
	commandString += "\" | tr a-z A-Z";
	//open second pipe
	upcaseList = popen(commandString.c_str(), "r");//runs unix command to convert the buffer to upcase
	//copy the results of the upcase to a string
	while (fgets(buffer, sizeof buffer, upcaseList) != NULL) {
		finalResultList += buffer;
	}
	
	printf("All files ending in .C in upcase including subdirectories:\n %s \n", finalResultList.c_str());
	pclose(upcaseList);
	pclose(fileList);//safely close memory opened earlier in the program
	return 0;
}