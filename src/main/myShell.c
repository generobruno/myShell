#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../inc/headers.h"

void checkScript(char **argv);

/**
 * @brief Función principal que corre el loop
 * 
 * @param argc Cantidad de argumentos de comando
 * @param argv Lista de argumentos de comando
 * @return int 0 si no hubo error
 */
int main(int argc, char **argv) {

    // Chequeamos si se puede correr el argumento que se haya pasado.
    if(argv[1] != NULL){
        checkScript(argv);
    }

    // Corremos el loop principal
    main_loop(argv);

    return 0;
}

/**
 * @brief Funcion que revisa si se le paso como argumento
 * un archivo .txt, en cuyo caso permite que se ejecute 
 * este archivo como un batch file para el programa
 * 
 * @param argv Argumento a revisar
 */
void checkScript(char **argv) {
    int len = strlen(argv[1]);              // Largo del argumento
    const char *last = &argv[1][len-4];
    if(len > 0){
        last = &argv[1][len-4];             // Ultimos 4 digitos del argumento
    } 
    if((strcmp(last,".txt") != 0)){
        fprintf(stdout,"\nSolo se permiten correr .txt\n");
        exit(1);
    }
    fprintf(stdout,"\nEjecutando Script: %s\n",argv[1]);
    sleep(1);
}