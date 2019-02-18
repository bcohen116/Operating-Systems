#include <unistd.h>// For popen/pclose
#include <stdio.h>//for printf
#include <stdlib.h>//for exit
#include <string>

using namespace std;

///<summary>
///What does this do:
/// Using 2 pipes and popen
/// Find all file names starting in this directory that are c files
/// Use recursion to find file names in subdirectories
/// Display the result in all upper case using another popen
///</summary>
int main(int argc, char* argv[]) {
	int pipe1[2], pipe2[2];
	char buffer[256];
	string resultList;
	pipe(pipe1);//open pipe1
	pipe(pipe2);//open pipe2
	
	FILE * fileList = popen("find . -name \\*.c -print | tr a-z A-Z", "r");//runs unix command to find all files ending in .c in this directory
	//read the contents of popen into a buffer, then save to a string
	while (fgets(buffer, sizeof buffer, fileList) != NULL) {
		resultList += buffer;
	}
	
	printf("Result: %s", resultList.c_str());
	//We have all of the files names, now we have to convert them
	//FILE * uppercaseList = popen(resultList.c_str() + "| tr a-z A-Z", "r");//convert file names to upper case
	
	pclose(fileList);//safely close memory opened earlier in the program
	return 0;
}