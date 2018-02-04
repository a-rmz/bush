#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils/types.h"

// Children proceses to be created
#define INTENDED_CHILDREN 6


int main(int argc, char ** argv) {
  //PID array for keeping track of children
  pid_t * children;
  children = malloc(INTENDED_CHILDREN * sizeof(pid_t));

  //Control variables
  int i;
  int childen_tracker = 0;
  pid_t pid; //current process PID
  int status; //child process exit status
  bool shutdown = false;

  //child proceses initialisation
  for(i = 0; i < INTENDED_CHILDREN; i++) {
    pid = fork();
    if(pid == 0) {
      execl("/usr/bin/xterm","-e","./bin/getty", (char*) NULL);
    } else{
      children[childen_tracker++] = pid;
    }
  }

  //main loop
  while(!shutdown) {
    //check children individually
    for(childen_tracker = 0; childen_tracker < INTENDED_CHILDREN; childen_tracker) {
      if(waitpid(children[childen_tracker], &status, WNOHANG) != 0) {
        //TODO check if double check is redundant
        if(WIFEXITED(status)) {
          //TODO properly check status to start shutdown procedure
          //TODO investigar SIGTERM y SIGKILL

          //restart child process
          pid = fork();
          if(pid == 0) {
            execl("/usr/bin/xterm","-e","./bin/getty",(char*)NULL);
          } else {
            printf("Process %d finished. Spawining process %d\n",
                children[childen_tracker], pid);
            children[childen_tracker] = pid;
          }
        }
      }
    }
  }
}
