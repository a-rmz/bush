#include <stdio.h>
#include "utils/io.h"

#define puts(x) printf("%s\n", x)

int main(int argc, char ** argv) {
  puts("Hello to bush!"); 

  char * user = prompt("user: ");
  char * password = prompt("user: ");

  puts(user);
  puts(password);
  
  return 0;
}
