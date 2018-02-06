#include <stdio.h>
#include <unistd.h>

#define CICLOS 250000
//useless program to test while getty is not ready
int main(){
    int i,j;

    pid_t pid = getpid();

    for(i=0;i<CICLOS;i++)
        for(j=0;j<CICLOS;j++);

    printf("proceso %d terminado\n", pid);
    return -1;
}
