#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "utils/io.h"
#include "utils/types.h"

#define ARGC 5

char * PATH;
// Prompt string
char * PS = "bush $ ";

typedef struct {
  char * exec;
  char ** args;
  int argc;
} command;

void set_path();
void handleSignal(int);
command * parse_input(char*);
bool is_command(command*, char*);

int main(int argc, char ** argv) {

  pid_t initPid;
  if(argc > 1){
    initPid = atoi(argv[1]);
  }else{
    exit(EXIT_FAILURE);
  }
  set_path();
  signal(SIGTERM,handleSignal);
  //kill(initPid,SIGUSR1);
  while(true) {
    char * input = prompt(PS);
    kill(initPid,SIGUSR1);
    command * c = parse_input(input);
    int i;
    for(i = 0; i < c->argc; i++) puts(c->args[i]);
    execv(c->exec, c->args);
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
    int argcount = 0;

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

void handleSignal(int signal){
  //TODO Handle exit properly
  printf("exiting\n");
  int i;
  for(int i = 0; i<100;i++){
    printf("qwerty\n");
  }
  exit(EXIT_SUCCESS);
}
