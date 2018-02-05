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

int main(int argc, char ** argv) {
  //PID array for keeping track of children
  //pid_t * children;
  //children = malloc(INTENDED_CHILDREN * sizeof(pid_t));
  char* curentPid; //string representation of current PID
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
    }
  }


  signal(SIGUSR1, terminationHandler);

  //main loop
  while(!shutdown) {
    if(waitpid(-1, &status, WNOHANG) > 0) {
      //TODO check if double check is redundant
      if(WIFEXITED(status)) {
        //TODO properly check status to start shutdown procedure
        //TODO investigar SIGTERM y SIGKILL

        //restart child process
        pid = fork();
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
  printf("a\n");
}
