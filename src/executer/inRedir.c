#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../../inc/headers.h"

/**
 * @brief 
 * 
 * @param parsed Comando a ejecutar
 * @param parsedPipe Archivo a donde se guarda la salida del comando
 */
void inRedir(char **parsed, char **parsedPipe) {
    // Abrimos un archivo para escribir. Se crea si no existe
    int infd = open(parsedPipe[0], O_RDONLY, 0777);
    if(infd < 0) {
        perror("\nError de open()");
        return;
    }

    // Creamos un hijo
    pid_t p = fork();
    if(p < 0) {
        perror("\nError al crear proceso hijo");
        return;
    }

    if(p == 0) { // Proceso hijo
        // Redirigimos el stdout del hijo a el archivo abierto
        if( dup2(infd,STDIN_FILENO) < 0 ) {
            perror("\nError de dup2");
            return;
        }
        // Cerramos el fd que ya no necesitamos
        close(infd);

        // Ejecutamos el comando
        execvp(parsed[0],parsed);
        perror("\nError de execvp.");
        return;
    }

    // Cerramos el fd, el padre no lo necesita
    close(infd);

    // Esperamos a que se termine el proceso
    waitpid(p,NULL,0);

    return;
}