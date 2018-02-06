#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "utils/io.h"
#include "utils/types.h"

//TODO call kill(init_pid,SIGUSR1); to shutdown everything
//TODO exit should only kill THIS process

#define ARGC 5

char * PATH;
// Prompt string
char * PS = "bush $ ";
// Parent PID made global for ease of use in the programm
pid_t init_pid;

typedef struct {
  char * exec;
  char ** args;
  int argc;
} command;

void set_path();
void handle_signal(int);
command * parse_input(char*);
bool is_command(command*, char*);

int main(int argc, char ** argv) {

  //Parse parent PID from args
  //Parent PID must be passed in args because of xterm
  if(argc > 1) {
    init_pid = atoi(argv[1]);
  } else {
    exit(EXIT_FAILURE);
  }
  set_path();

  //Activates signal monitor
  signal(SIGTERM, handle_signal);

  while(true) {
    char * input = prompt(PS);
    command * c = parse_input(input);
    pid_t  child_pid = fork();

    if(child_pid == 0) {
      execv(c->exec, c->args);
      // Wait only if the process must not be executed
      // in background
    } else {
      wait(NULL);
    }
  }

  return 0;
}

void set_path() {
  PATH = getenv("PATH");
}

command * parse_input(char * input) {
  char * token = strtok(input, " ");
  if(token != NULL) {
    command * c = (command *) malloc(sizeof(command *));
    char * command = token;

    char ** args = (char **) malloc(sizeof(char*) * ARGC);
    // the first argument argv[0] must be the program name
    int argcount = 0;
    args[argcount++] = command;

    while(token != NULL) {
      token = strtok(NULL, " ");
      args[argcount++] = token;
    }
    if(argcount == 0) {
      args[0] = "";
    }
    argcount--;

    c->exec = command;
    c->args = args;
    c->argc = argcount;
    return c;
  } else {
    return NULL;
  }
}

bool is_command(command * c, char * str) {
  return strcmp(c->exec, str) == 0 ? true : false;
}

//this get called after recieving SIGTERM
void handle_signal(int signal) {
  //TODO Handle exit properly
  exit(EXIT_SUCCESS);
}
