#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//numero de procesos hijos que seran creados
#define INTENDED_CHILDREN 6

//bool no existe en C yay!
typedef int bool;
#define true 1
#define false 0

//punto de entrada, no necesito los argumentos pero ya estaban en el template y noafectan en nada
int main(int argc, char ** argv) {
    //arreglo con los PIDs de los procesos hijos
    pid_t* children;
    children = malloc(INTENDED_CHILDREN * sizeof(pid_t));

    //variables de control
    int i; //good ol' reliable i
    int childen_tracker = 0; //pude haber usado i pero es mas facil saber que estoy buscando con esta variable
    pid_t pid; //PID del proceso
    int status; //status de como termino el proceso
    bool shutdown = false; //checa si ya terminamos el experimento

    //creacion de procesos hijos inicial similar a "arranca.c" de la practica 1
    for(i = 0; i < INTENDED_CHILDREN; i++){
        pid = fork();
        if(pid == 0){
            execl("/usr/bin/xterm","-e","./time",(char*)NULL);
        } else{
            children[childen_tracker++] = pid;
        }
    }

    //mientras no haya terminado sigue por siempre
    while(!shutdown){
        //verifica  cada hijo eventualmente
        for(childen_tracker = 0; childen_tracker < INTENDED_CHILDREN;childen_tracker++){
            //Si ya termino el proceso lo reinicia
            if(waitpid(children[childen_tracker], &status, WNOHANG) != 0){
                //TODO Probar redundancia de doble chequeo
                if(WIFEXITED(status)){
                    //TODO condicion para recibir estado de error y iniciar procedimiento de shutdown
                    //TODO investigar SIGTERM y SIGKILL

                    //TODO extraer a metodo independiente para evitar repetir codigo?
                    //Si no hace falta hacer shutdown se crea proceso nuevo
                    pid = fork();
                    if(pid == 0){
                        execl("/usr/bin/xterm","-e","./time",(char*)NULL);
                    } else {
                        printf("Proceso %d terminado iniciando nuevo proceso %d\n", children[childen_tracker],pid);
                        children[childen_tracker] = pid;
                    }
                }
            }
        }
    }
}
