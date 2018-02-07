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

//default commands
char * exit_program = "exit";
char * shutdown = "shutdown";

//helping strings
char * ampersend = "&";

//usefull little flag
bool is_background = false;
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
  }else{
    exit(EXIT_FAILURE);
  }
  set_path();

  //Activates signal monitor
  signal(SIGTERM,handle_signal);

  while(true) {
    char * input = prompt(PS);
    command * c = parse_input(input);
    if(is_command(c,exit_program)){
      printf("exiting\n");
      exit(0);
    }
    if(is_command(c,shutdown)){
      kill(init_pid,SIGUSR1);
      exit(0);
    }
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
  is_background = false;
  char * token = strtok(input, " ");
  if(token != NULL) {
    command * c = (command *) malloc(sizeof(command *));
    char * command = token;

    char ** args = (char **) malloc(sizeof(char*) * ARGC);
    int argcount = 0;
    while(token != NULL) {
      token = strtok(NULL, " ");
      /*if(strcmp(token,ampersend) != 0){*/
        args[argcount++] = token;
      /*} else{
        is_background = true;
      }*/
    }
    if(argcount == 0) {
      args[0] = "";
    }
    argcount--;
    //puts(args[argcount]);
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
