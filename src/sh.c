#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "utils/io.h"
#include "utils/types.h"


#define ARGC 5
#define AMP "&"

char * PATH;
// Prompt string
char * PS = "bush $ ";

//default commands
char * exit_program = "exit";
char * shutdown = "shutdown";

//flag to check if process should block or not
bool is_background = false;

// Parent PID made global for ease of use in the programm
pid_t init_pid;

/*
 * Struct to handle all the information
 * related to a command
 */
typedef struct {
  char * exec;
  char ** args;
  int argc;
} command;

void set_path();
void handle_signal(int);
command * parse_input(char*);
bool is_command(command*, char*);
bool exec(command*);

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

  //good ol' fork variables
  pid_t child_pid;
  int status;

  while(true) {
    char * input = prompt(PS);
    command * c = parse_input(input);

    if(is_command(c, exit_program)) {
      printf("Closing bush\n");
      exit(0);
    }
    if(is_command(c,shutdown)) {
      kill(init_pid, SIGUSR1);
      exit(0);
    }

    child_pid = fork();

    if(child_pid == 0) {
      //execute command on child process
      if(!exec(c)) {
        printf("bush: command not found: %s\n", c->exec);
      }
      // Wait only if the process must not be executed
      // in background
    } else {
      //if is_background is set to false blocks sh to wait for children
      if(is_background == false){
        wait(&status);
        continue;
      }

    }
  }

  return 0;
}

// Load the PATH from the parent
void set_path() {
  PATH = getenv("PATH");
}

/*
 * Try to execute the command
 * If it cannot be executed, tries with every directory
 * listed under the path
 *
 * return: success or failure
 */
bool exec(command * c) {
  if(execv(c->exec, c->args) < 0) {
    char * _PATH = (char *) malloc(sizeof(char*) * strlen(PATH));
    strcpy(_PATH, PATH);
    char * dir = strtok(_PATH, ":");

    while(dir != NULL) {
      char * full_path  = (char *) malloc(sizeof(char*) * (strlen(c->exec) + strlen(dir)));
      sprintf(full_path, "%s/%s", dir, c->exec);
      // If the executable is not in the path
      // pass to the next directory
      if(execv(full_path, c->args) > -1) return true;
      dir = strtok(NULL, ":");
    }
    free(_PATH);
  }
  return false;
}

command * parse_input(char * input) {
  is_background = false;
  char * token = strtok(input, " ");
  if(token != NULL) {
    command * c = (command *) malloc(sizeof(command *));
    char * command = token;

    char ** args = (char **) malloc(sizeof(char*) * ARGC);
    // the first argument argv[0] must be the program name
    int argcount = 0;
    args[argcount++] = command;

    //starts reading args, if none is present saves an empty string for execv
    token = strtok(NULL," ");
    while(token != NULL) {
      //if argument is & the flag is set and the argument skipped
      //else the argument is stored
      if(strcmp(token, AMP) == 0) {
        is_background = true;
      } else {
        args[argcount++] = token;
      }
      token = strtok(NULL, " ");
    }
    if(argcount == 0) {
      args[0] = "";
      argcount--;
    }

    //save data on pointer
    c->exec = command;
    c->args = args;
    c->argc = argcount;
    return c;
  } else {
    return NULL;
  }
}

/*
 * Compare a given command with its string representation
 */
bool is_command(command * c, char * str) {
  return strcmp(c->exec, str) == 0 ? true : false;
}

/*
 * Handle the receiving of the SIGTERM signal
 */
void handle_signal(int signal) {
  //no cleanup needed, we just finish the programm
  exit(EXIT_SUCCESS);
}
