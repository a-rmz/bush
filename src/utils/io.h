#ifndef IO_H_
#define IO_H_

#include <stdio.h>

#define BASE_STR_SIZE 30

#define puts(x) printf("%s\n", x)
#define putd(x, y) printf("%s: %d\n", x, y)

char * prompt(char*);
void save_credentials(char*, char*, char*);

#endif
