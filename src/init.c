#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "utils/types.h"

// Children proceses to be created
#define INTENDED_CHILDREN 6

void termination_handler(int);

int main() {
  char curent_pid[10]; //string representation of current PID
  sprintf(curent_pid,"%d",getpid());

  //Control variables
  int i;
  pid_t pid; //current process PID
  int status; //child process exit status
  bool shutdown = false;

  //child proceses initialisation
  for(i = 0; i < INTENDED_CHILDREN; i++) {
    pid = fork();
    if(pid == 0) {
      execl("/usr/bin/xterm","xterm","-e","./bin/getty", curent_pid, (char*) NULL);
    }
  }


  signal(SIGUSR1, termination_handler);

  //main loop
  while(!shutdown) {
    //checks all children simultaneously
    if(waitpid(-1, &status, WNOHANG) > 0) {
      if(WIFEXITED(status)) {

        pid = fork();
        if(pid == 0) {
          //exec getty on children
          execl("/usr/bin/xterm","xterm","-e","./bin/getty", curent_pid, (char*)NULL);
        } else {
          //debug message on parent
          printf("Child process finished. Spawining new process\n");
        }
      }
    }
  }
}

// Upon recieving termination signal SIGUSR1
// kills all children processes
void termination_handler(int signum) {
  int status;
  int i;
  kill(-1 * getpid(), SIGTERM);

  for(i = 0; i < INTENDED_CHILDREN; i++) {
    wait(NULL);
  }

  exit(0);
}
