#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "utils/io.h"
#include "utils/types.h"

// define the location of the
// sh executable
#define SH_BIN "bin/sh"

bool login(char*, char*, char*);

int main(int argc, char ** argv) {
  puts("Hello to bush!"); 

  char * user = prompt("user: ");
  char * password = prompt("password: ");

  bool valid = login("passwd", user, password);

  if(valid) {
    puts("Sucessfully logged in");

    int status;
    pid_t pid = fork();
    // exec sh on child
    if(pid == 0) {
      execl(SH_BIN, "sh", NULL);
    // wait on parent
    } else {
      wait(&status);
      puts("Goodbye, human!");
    }
  } else {
    puts("Please enter valid credentials");
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
