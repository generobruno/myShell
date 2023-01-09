#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include "../../inc/headers.h"

// Funciones auxiliares
void clearAmpersand(char **line);
int countPipes(char *line);

/**
 * @brief Funcion que analiza un string ingresado para separarlo
 * en tokens, ya sea por pipe o espacio. Ejecuta un comando si
 * reconoce que se ha ingresado uno.
 * 
 * @param line String a analizar
 * @param parsed String tokenizado
 * @param parsedPipe Pipe tokenizada
 * @param numPipes Cantidad de pipes
 * @return int      0 en caso de ser un comando
 *                  1 en caso de ser otro token
 *                  2 en caso de ser token con pipe
 *                  3 en caso de haber backgroundExecution
 *                  4 en caso de I/O redirection (out)
 *                  5 en caso de I/O redirection (in)
 */
int parse_line(char *line, char **parsed, char **parsedPipe, char **parsedRedir, int *numPipes) {
    char *line_piped[MAXCOM];
    char *line_redir[MAXCOM];
    int piped = 0;
    int background = 0;
    int redirection = 0;

    // Analizamos si hay pipe y contamos la cant de pipes que puedan existir
    *numPipes = countPipes(line);
    piped = parsePipe(line, line_piped);    // Tengo a line_piped separada por |

    // Analizamos si hay stream redirection
    if(strstr(line," < ") != NULL){
        // Si hay redir in se devuelve 5
        redirection = parseRedir(line, line_redir, 1);
    }
    if(strstr(line," > ") != NULL){
        // Si hay redir out se devuelve 4
        redirection = parseRedir(line, line_redir, 2);
    }

    // No se soportan pipes y I/O redirection al mismo tiempo
    if(piped && redirection){
        fprintf(stdout,"\nNo soportado.");
        return 0;
    }

    // En caso de haber redireccion, separamos los argumentos
    if(redirection) {
        parseSpace(line_redir[0],parsed);
        parseSpace(line_redir[1],parsedRedir);

        return redirection;
    }

    // En caso de pipe, analizamos ambos conjuntos de tokens
    if(piped) {
        if(line_piped[0] == NULL){
            fprintf(stdout,"\nError, try again.");
            return 0;
        }
        // Analizamos si hay backround execution
        parseSpace(line_piped[0], parsed);                  // Primer arg             
        //parseSpace(line_piped[1], parsedPipe); 
        for(int i = 0; i < MAXCOM; i++) {             // Resto de args    
            parsedPipe[i] = line_piped[i+1];
            // Cuando llegue al fin de line, termina
            if(parsedPipe[i] == NULL) {
                background = parseBackground(line_piped[i]);    // Analiza el pipe final
                break;
            }
            // En caso de haber muchos espacios
            if(strlen(parsedPipe[i]) == 0) 
                i--;
        }
    } else {
        // Analizamos si hay backround execution
        background = parseBackground(line);
        parseSpace(line, parsed);
    }

    // Si no se ingresa nada, se ignora
    if(parsed[0] == NULL){
        return 0;
    }

    // En caso de background execution
    if(background) {
        // Eliminamos el caracter '&' de los argumentos
        clearAmpersand(parsed);
        
        // Igual en caso de haber pipe
        if(piped) {
            clearAmpersand(parsedPipe);
            return 1 + piped;
        } 

        return 3;
    } // Fin parsing background


    // Ejecutamos comando en caso de poder
    if(cmdHandler(parsed)) {
        return 0;
    } else {
        return 1 + piped;
    }
}

/**
 * @brief Funcion que se encarga de reemplazar el
 * ultimo caracter '&' por '\0'.
 * 
 * @param line string a filtrar
 */
void clearAmpersand(char **line) {
    for(int i = 0; line[i] != NULL; i++) {
            if(strstr(line[i],"&")){
                line[i] = '\0';
            }
        }
}

/**
 * @brief Funcion que cuenta el numero de pipes que 
 * hay en un string, definidos por el caracter '|'
 * 
 * @param line String a analizar
 * @return int Cantidad de pipes
 */
int countPipes(char *line) {
    int numpipe = 0;
    int i = 0;
    while(line[i] != '\0'){
        if(line[i] == '|') {
            numpipe++;
        }
        i++;
    }

    return numpipe;
}
