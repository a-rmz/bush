#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils/io.h"
#include "utils/types.h"

// Define the location of the
// sh executable
#define SH_BIN "bin/sh"

bool login(char*, char*, char*);
void handleTermSignal(int);
void handleSignal(int);

// Parent PID made global for ease of use in the programm
pid_t initPid;

int main(int argc, char ** argv) {

  //Parse parent PID from args
  //Parent PID must be passed in args because of xterm
  pid_t childrenPID;
  if(argc > 1){
    initPid = atoi(argv[1]);
  }else{
    exit(EXIT_FAILURE);
  }

  //string representation of current PID
  char curentPid[10];
  sprintf(curentPid,"%d",getpid());

  // Activates monitors for SIGUSR1 and SIGTERM
  signal(SIGUSR1, handleSignal);
  signal(SIGTERM, handleTermSignal);


  while(true){
    system("tput clear");
    puts("Hello to bush!");

    char * user = prompt("user: ");
    char * password = prompt("password: ");

    bool valid = login("passwd", user, password);

    if(valid) {
      puts("Sucessfully logged in");
      int status;
      childrenPID=fork();
      if(childrenPID == 0){
        // exec sh on child
        execl(SH_BIN, "sh",curentPid,(char*) NULL);
      } else{
        // wait on parent
        wait(&status);
        puts("Goodbye, human!");
      }
    } else {
      puts("Please enter valid credentials");
    }
  }


  return 0;
}

bool login(char * passwd_file, char * user, char * password) {
  // Don't accept in case any of both are empty
  if(strlen(user) < 1 || strlen(password) < 1) return false;

  // +1 for the :, +1 for the '\0'
  unsigned int length = strlen(user) + strlen(password) + 2;
  char * credentials = (char *) malloc (sizeof(char) * length);
  sprintf(credentials, "%s:%s\n", user, password);

  FILE * f;
  char * line = NULL;
  size_t len = 0;
  ssize_t size;

  f = fopen(passwd_file, "r");
  // Problems opening the file
  if(f == NULL) {
    puts("Error opening the login file. Please contact your sysadmin");
    exit(EXIT_FAILURE);
  };

  while((size = getline(&line, &len, f)) != -1) {
    if(strcmp(credentials, line) == 0) return true;
  }

  free(line);
  return false;
}

// Upon recieving SIGTERM kills children processes
void handleTermSignal(int signal){
  int status;
  int i;
  for(int i = 0; i<10000;i++)
    printf("term\n");
  kill(-1*getpid(),SIGTERM);
  wait(&status);
  exit(EXIT_SUCCESS);
}

// Upon recieving SIGUSR1 propagates it upwards to init
void handleSignal(int signal){
  kill(initPid,SIGUSR1);
}
