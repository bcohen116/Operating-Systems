#include <stdio.h>//standard io
#include <string.h>//for string commands
#include <unistd.h>//for unix commands
#include <stdlib.h>//for strlen
#include <iostream>//for stio
#include <sys/wait.h>//for wait funcitons

using namespace std;

//Instructions of assignment:
// Check inputs for errors (input must be a filename that exists)
//Create 3 child forks
// Enter N number of loops and display:
//  Each process (both parent and children) display PID, or PID + Parent process
//  Repeat every 2 seconds
int main(int argc, char* argv[])
{
  int ppid,pid,n;//ppid=parent,pid=child
  //check if input is correct
  if (argc != 2) {
    printf("Usage: %s N (N must be a number)\n", argv[0]);
    exit(1);
  }
  else{
    //Input has correct # of parameters, now check is it's a number
    for (int z = 0; z < strlen(argv[1]);z++){
      //loop through each char in the input
      if (argv[1][z] < 48 || argv[1][z] > 56){
        //if this is true then the character is NOT a number because of char values
        printf("You did not type a number. Usage: %s N (N must be a number)\n", argv[0]);
        exit(1);
      }
    }
    //If the code gets to this line, then input is indeed a number
    try{
      n = stoi(argv[1]);//convert input into an int
    }
    catch(const std::invalid_argument){
      //Just in case catch statements
      printf("You did not use a number. Usage: %s N (N must be a number)\n", argv[0]);
      exit(1);
    }
    catch(const std::out_of_range){
      printf("Number was too large. Usage: %s N (N must be a number)\n", argv[0]);
      exit(1);
    }
  }

  ppid = getpid();//retreive the parent process ID
  for (int y = 0; y < 3; y++){
    //This loop should execute 3 times, creating 3 child processes total
    pid = fork();
    // printf("PID created: %d \n", pid);//For debugging reasons
    if (pid == ppid || pid > 0){
      //If this line runs, we are at a parent process
      continue;
    }
    else if (pid != ppid && pid >=0){
      //A Child process is running.
      //In this case we break because we don't want to keep duplicating processes
      break;
    }
    else{
      //fork failed
      perror("Forking a process failed... \n");
    }
  }
  for (int x = 0; x < n; x++){
    if (pid == ppid || pid > 0){
      //If this line runs, we are at a parent process
      pid = getpid();//make sure we have the correct ID number
      printf("This is the main process, my PID is %i \n",pid);
    }
    else if (pid == 0){
      //A Child process is running.
      int parent = getppid();//get parent ID, should be the same as the ppid variable above
      printf("This is a child process, my PID is %d, my parent PID is %d \n",getpid(),parent);
    }
    else{
      perror("This should never happen, something is very wrong with a fork. \n");
    }
    sleep(2);
  }

  wait(NULL);//wait for children to finish doing everything
  return 0; //end program
}
