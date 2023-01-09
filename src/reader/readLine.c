#include <stdio.h>
#include <stdlib.h>
#include "../../inc/headers.h"

#define BUFF_SIZE 1024

/**
 * @brief Funcion que se encarga de leer una linea
 * de texto que ingresemos por consola.
 * 
 * @return char* Lo que se ingrese por stdin
 */
char *read_line(void) {
    int buffsize = BUFF_SIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * buffsize);
    int c;

    if(!buffer) {
        fprintf(stderr, "Malloc Error\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Leemos un caracter desde stdin
        c = getchar();

                                    // Agregamos los caracteres al buffer
        if((c == EOF) | (c == '\n')) {  // Cuando se llegue al final, se reemplaza el ultimo c con un espacio
            buffer[pos] = '\0';
            return buffer;          // y se devuelve el buffer
        } else {
            buffer[pos] = c;
        }
        pos++;
                                    // Si excedemos el buffer, realocamos
        if(pos >= buffsize) {
            buffsize += BUFF_SIZE;
            buffer = realloc(buffer, buffsize);
            if(!buffer){
                fprintf(stderr, "Malloc Error\n");
                exit(EXIT_FAILURE);
            }
        }

    } // Fin del while

}