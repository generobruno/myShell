#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que analiza un string para ver 
 * si contiene el caracter "&", con el cual se 
 * identifica que la shell debe correr el comando 
 * pasado en background.
 * 
 * @param line String a analizar
 * @return int 1 si hay que realizar backround execution
 */
int parseBackground(char* line) {
    int len = strlen(line);              // Largo del argumento
    const char *last_one = NULL;
    if(len > 0){
        last_one = &line[len-1];         // Ultimo caracter
    } else {
        return 0;
    }
    // Chequeamos el ultimo caracter de la linea para ver si tiene '&'
    if((strcmp(last_one,"&") != 0)){
        return 0;
    } else {
        return 1;
    }
}