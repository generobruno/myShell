#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que redirige el output del comando que ejecutemos
 * con parsed al archivo que especifiquemos con parsedPipe
 * 
 * @param parsed Comando a ejecutar
 * @param parsedPipe Archivo a donde se guarda la salida del comando
 */
void outRedir(char **parsed, char **parsedPipe) {
    // Abrimos un archivo para escribir. Se crea si no existe
    int outfd = open(parsedPipe[0], O_CREAT | O_WRONLY, 0777);
    if(outfd < 0) {
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
        if( dup2(outfd,STDOUT_FILENO) < 0 ) {
            perror("\nError de dup2");
            return;
        }
        // Cerramos el fd que ya no necesitamos
        close(outfd);

        // Ejecutamos el comando
        execvp(parsed[0],parsed);
        perror("\nError de execvp.");
        return;
    }

    // Cerramos el fd, el padre no lo necesita
    close(outfd);

    // Esperamos a que se termine el proceso
    waitpid(p,NULL,0);

    return;
}