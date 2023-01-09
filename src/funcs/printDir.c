#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que imprime el directorio actual,
 * el nombre del usuario y del host por consola.
 * 
 */
void printDir(void) {
    // Obetemos el directorio actual
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    // Obtenemos el nombre
    char *username = getenv("USER");
    // Obtenemos el host
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    // Imprimimos la informacion
    fprintf(stdout, ANSI_COLOR_GREEN "%s@" 
    ANSI_COLOR_GREEN "%s:" ANSI_COLOR_CYAN "~%s$ " ANSI_COLOR_RESET,
    username, hostname, cwd);
}