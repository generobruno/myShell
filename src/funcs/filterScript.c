#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion utilizada para filtrar el script
 * 
 * @param filter linea a filtrar
 * @return char* linea filtrada
 */
char *filterScript(char *filter) {

    char *filter1 = strsep(&filter, "\n");
    char *line = strsep(&filter1, "\t");

    return line;
}