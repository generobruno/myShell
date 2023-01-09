#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../../inc/headers.h"

int CHILD;

void handleSigStp(int signal);
void handleSigInt(int signal);
void handleSigQuit(int signal);

/**
 * @brief Funcion que crea un proceso hijo a partir de los 
 * argumentos que le pasemos como parametro. Al usar execvp,
 * el programa a ejecutar debe encontrarse en el path actual.
 * 
 * @param parsed Nombre del programa a ejecutar y sus comandos.
 */
void execSimple(char **parsed) {
    // Creamos un proceso hijo
    pid_t child = fork();
    CHILD = child;
    int status;

    switch (child)
    {
    case -1:    // Caso de error
        fprintf(stdout,"\nError creando el proceso.\n");
        return;
    case 0:     // Caso del hijo
        // parsed[0] = Nombre del programa, parsed = Lista de argumentos
        execvp(parsed[0], parsed);
        fprintf(stderr,"\nNo se pudo ejecutar el comando.\n");
        abort();
    default:    // Caso del padre
        // Manejamos algunas de las señales
        signal(SIGTSTP,&handleSigStp);
        signal(SIGINT,&handleSigInt);
        signal(SIGQUIT,&handleSigQuit);
        // El padre espera a que sus hijos terminen o mueran
        waitpid(child,&status,WUNTRACED);
        return;
    }
}

/**
 * @brief Funcion que maneja la señal SIGTSTP
 * para cerrar el trabajo en primer plano.
 * 
 * @param signal 
 */
void handleSigStp(int signal) {
    fprintf(stdout,"Deteniendo %d",CHILD);
    fflush(stdout);

    kill(CHILD,SIGTSTP);
}

/**
 * @brief Funcion que maneja la señal SIGINT
 * para interrumpir el trabajo en primer plano.
 * 
 * @param signal SIGINT
 */
void handleSigInt(int signal) {
    fprintf(stdout,"Interrumpiendo %d\n", CHILD);
    fflush(stdout);

    kill(CHILD,SIGTSTP);
    return;
}

/**
 * @brief Funcion que maneja la señal SIGQUIT
 * para abortar el trabajo en ejecucion.
 * 
 * @param signal 
 */
void handleSigQuit(int signal){
    fprintf(stdout,"Cerrando %d\n", CHILD);
    fflush(stdout);

    abort();
}

