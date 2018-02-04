#include "io.h"
#include "types.h"
#include <stdlib.h>

#define BASE_STR_SIZE 30

/*
 * Prompt the user for an input
 *
 * String memory management algorithm taken from:
 * https://www.mkyong.com/c/how-to-handle-unknow-size-user-input-in-c/
 */
char * prompt(char * p) {
  unsigned int str_length = BASE_STR_SIZE; 
  char * str = (char *) malloc(str_length);

  printf("%s", p);  

  int c = EOF;
  unsigned int i = 0;

  while ((c = getchar()) != '\n' && c != EOF) {
    str[i++] = (char) c;
    if(i == str_length) {
      str_length += BASE_STR_SIZE;
      str = (char *) realloc(str, str_length); 
    }
  }

  // End of string
  str[i] = '\0';
  return str;
};

void save_credentials(char * file, char * user, char * password) {
  FILE * f = fopen(file, "a");
  fprintf(f, "%s:%s\n", user, password);
  fclose(f);
}

