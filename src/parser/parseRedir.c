#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que separa un string en argumentos
 * antes y despues de el caracter '<' o '>'. Retorna un
 * valor entero dependiendo de cual de estos 2 caracteres
 * se encuentre.
 * 
 * @param line String a separar
 * @param line_redir Array donde se guardan los args
 * @param ref Para ver si separar por > o <
 * @return int      4 en caso de I/O redirection (out)
 *                  5 en caso de I/O redirection (in)
 */
int parseRedir(char* line, char** line_redir, int ref) {

    if(ref == 1) {

        // Analizamos para >
        for(int i = 0; i < MAXCOM; i++) {
            line_redir[i] = strsep(&line, "<");
            if(line_redir[i] == NULL) {
                break;
            }
        }

        for(int i = 0; i<MAXCOM; i++){
            if(line_redir[i] == NULL) {
                break;
            }
        }

        // Si no se encontro... 
        if(line_redir[1] == NULL) {
            return 0;
        } else { // En caso de encontrar <, devuelve 1
            return 5;
        }

    } else if (ref == 2) {
        // Analizamos para >
        for(int i = 0; i < MAXCOM; i++) {
            line_redir[i] = strsep(&line, ">");
            if(line_redir[i] == NULL) {
                break;
            }
        }

        for(int i = 0; i<MAXCOM; i++){
            if(line_redir[i] == NULL) {
                break;
            }
        }

        // Si no se encontro... 
        if(line_redir[1] == NULL) {
            return 0;
        } else { // En caso de encontrar >, devuelve 4
            return 4;
        }
    } else {
        fprintf(stdout,"\nError parseRedir.");
        return 0;
    }  

}
