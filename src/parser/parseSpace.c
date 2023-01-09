#include <stdio.h>
#include <string.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que analiza un string para ver los
 * comandos escritos en el mismo. Los mismos se van 
 * agregando a parsed.
 * 
 * @param line String a analizar
 * @param parsed Comandos (o palabras) encontrados.
 */
void parseSpace(char *line, char **parsed) {
    for(int i = 0; i < MAXCOM; i++) {
        parsed[i] = strsep(&line, " ");

        // Cuando llegue al fin de line, termina
        if(parsed[i] == NULL) 
            break;
        // En caso de haber muchos espacios
        if(strlen(parsed[i]) == 0) 
            i--;
    }
}