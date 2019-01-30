#include <stdio.h>//standard io
#include <string.h>//for string commands
#include <unistd.h>//for unix commands and open/close functions
#include <stdlib.h>//for strlen
#include <iostream>//for stio
#include <sys/wait.h>//for wait funcitons

using namespace std;

//Written by Ben Cohen
//Instructions of assignment:
// Check inputs for errors
//Create 3 child forks
//Display Parent info one time
// One child uses ls -l command
// Another child uses ps -ef command
// Final child displays content of a txt file that exists
//Parent displays "main process terminates" when all of that is done
int main(int argc, char* argv[])
{
  int ppid,n;//ppid=parent,pid=child
  int pid[4];//array of process identifiers
  //check if input is correct
  if (argc != 2) {
    printf("Usage: %s file_name (your file must exist)\n", argv[0]);
    exit(1);
  }
  else{
    //Input has correct # of parameters, now check if file exists
    // string sourceName = std::to_string(argv[1]);
    FILE * source = fopen(argv[1], "r");
    if (source == NULL){
      //The file could not open correctly, throw an error
      fprintf(stderr, "Correct usage: %s file_name (with a file that exists). Your error on line %d was: %s\n",argv[0],__LINE__,strerror(errno));
      exit(1);
    }
    else{
      fclose(source);//we don't need the file anymore because other functions will access it later
    }
  }

  ppid = getpid();//retreive the parent process ID
  for (int y = 0; y < 3; y++){
    //This loop should execute 3 times, creating 3 child processes total
    pid[y] = fork();
    // printf("PID created: %d \n", pid);//For debugging reasons
    if (getpid() == ppid){
      //If this line runs, we are at a parent process
      continue;
    }
    else if (getpid() != -1){
      //A Child process is running.
      //In this case we break because we don't want to keep duplicating processes
      break;
    }
    else{
      //fork failed
      perror("Forking a process failed... \n");
    }
  }
  //Children should have been created at this point, now run actions based on what process we see
  int currentPid = getpid();
  if (currentPid == ppid){
    //If this line runs, we are at a parent process
    currentPid = getpid();//make sure we have the correct ID number
    printf("This is the main process, my PID is %i \n",ppid);
  }
  else{
    //This is a child process, check which one it is
    //Since we ran pid[y] = fork(), then we can keep track of which child process is currently running
    //Only the current child being executed will show up as 0, so we use that to check
    if (0 == pid[0]){
      //run ls -l command
      execl( "/bin/ls", "ls", "-l", (char*)0 );//input of path, commands, ending param
      return 0;//end the child process
    }
    else if (0 == pid[1]){
      //run ps -ef command
      execl( "/bin/ps", "ps", "-ef", (char*)0 );//input of path, commands, ending param
      return 0;//end the child process
    }
    else if (0 == pid[2]){
      //display file contents
      execl("/bin/more", "more", argv[1], (char*)0 );//input of path, commands, ending param
      return 0;//end the child process
    }
    else{
      //Something went wrong with our ID naming
      perror("PID names did not match an open process. \n");
    }
  }
  pid_t finishedPid;
  while (finishedPid > 0){
    finishedPid = wait(NULL);//wait for children to finish doing everything
    if (finishedPid > 0){
      printf("Child PID %d Finished running \n \n",finishedPid);
    }
  }
  printf("main process terminates \n");
  return 0; //end program
}
