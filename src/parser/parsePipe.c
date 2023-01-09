#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que analiza un string para ver
 * si contiene el caracter "|", con el cual se 
 * identifica un pipe.
 * 
 * @param line String a analizar
 * @param line_piped String antes de "|"
 * @return int 1 si hay pipe
 */
int parsePipe(char* line, char** line_piped) {
    // Si se encuentra el caracter "|", la funcion strsep()
    // este se reemplaza con "\0", en line_piped[1] se guarda el texto
    // que le seguia a "|" y line_piped[0] es el texto hasta "|"
    for(int i = 0; i < MAXCOM; i++) {
        line_piped[i] = strsep(&line, "|");
        if(line_piped[i] == NULL) {
            break;
        }
    }

    /**
     * ie: echo algo | echo otra
     * line_piped[0]       =       "echo algo"
     * line_piped[1]       =       "echo otra"
     */


    if(line_piped[1] == NULL) {
        return 0;
    } else {
        return 1;
    }
}