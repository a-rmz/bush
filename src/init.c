#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "utils/types.h"

// Children proceses to be created
#define INTENDED_CHILDREN 6

void terminationHandler(int);

int main() {

  //PID array for keeping track of children
  //pid_t* children;
  //children = malloc(INTENDED_CHILDREN * sizeof(pid_t));
  char curentPid[10]; //string representation of current PID

  //Control variables
  int i;
  //int childen_tracker = 0;
  pid_t pid; //current process PID
  int status; //child process exit status
  bool shutdown = false;
  sprintf(curentPid,"%d",getpid());
  //child proceses initialisation
  for(i = 0; i < INTENDED_CHILDREN; i++) {
    pid = fork();
    if(pid == 0) {
      execl("/usr/bin/xterm","xterm","-e","./bin/getty",curentPid, (char*) NULL);
    } /*else{
      children[childen_tracker++] = pid;
    }*/
  }


  signal(SIGUSR1, terminationHandler);

  //main loop
  while(!shutdown) {
    pid_t returnedPid;
    if((returnedPid = waitpid(-1, &status, WNOHANG)) > 0) {
      if(WIFEXITED(status)) {
        /*for(childen_tracker = 0; childen_tracker<INTENDED_CHILDREN; childen_tracker++){
          if(children[childen_tracker] == returnedPid) break;
        }

        //restart child process
        children[childen_tracker]*/ pid = fork();
        if(pid == 0) {
          execl("/usr/bin/xterm","xterm","-e","./bin/getty",curentPid, (char*)NULL);
        } else {
          printf("Child process finished. Spawining new process\n");
        }
      }
    }
  }
}

void terminationHandler(int signum){
  /*int status;
  int i;
  kill(-1 * getpid(), SIGTERM);

  for(i = 0; i < INTENDED_CHILDREN; i++){
    wait(NULL);
  }

  exit(0);*/
  printf("%s\n", "SIGNAL RECIEVED");
}
