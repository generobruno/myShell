#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../inc/headers.h"

// Funciones Auxiliares
int cd(char **parsed);
void echo(char **parsed);

/**
 * @brief Funcion que ejecuta los distintos comandos
 * soportados por myShell.
 * 
 * @param parsed Linea con los comandos.
 * @return int 0 en caso de no haber error.
 */
int cmdHandler(char **parsed) {
    // Cantidad de comandos habilitados
    int cmdNum = 5;
    // Indice para el switch
    int switchIdx = 0;
    // Lista de comandos
    char *CmdList[cmdNum];

    // Lista explicita de comandos
    CmdList[0] = "cd";
    CmdList[1] = "clr";
    CmdList[2] = "echo";
    CmdList[3] = "quit";
    CmdList[4] = "help";

    // Vemos si parsed coincide con alguno de los comandos
    for(int i = 0; i < cmdNum; i++) {
        if(strcmp(parsed[0], CmdList[i]) == 0) {
            switchIdx = i + 1;
            break;
        }
    }

    // Aplicacion de los comandos
    switch (switchIdx) {
    case 1:     // "cd" 
        return cd(parsed);
    case 2:     // "clr"
        clear();
        return 1;
    case 3:     // "echo"
        echo(parsed);
        return 1;
    case 4:     // "quit"
        clear();
        fprintf(stdout,"\nNos vemos!\n\n");
        exit(0);
    case 5:     // "help"
        showHelp();
        return 1;
    default:
        break;
    }

    return 0;
}

/**
 * @brief Funcion change directory para myShell.
 * 
 * @param parsed Directorio a cambiar.
 * @return int 1 en caso de exito.
 */
int cd(char **parsed) {
    // En caso de faltar segundo argumento
    if(parsed[1] == NULL) {
        fprintf(stdout,"Falta argumento\n");
        return 1;
    }

    // Analizamos si hay error
    if (strcmp(parsed[1],"\0") == 0){
        fprintf(stdout, "\n Falta argumento.");
    } else if (parsed[1] == NULL) {
        fprintf(stdout,"\nERROR");
    } 

    // Variables para guardar los wd
    static char prev[256];
    static char aux[256];

    // Con la opcion "-" se vuelve al directorio anterior
    if(strcmp(parsed[1],"-") == 0) {
        getcwd(aux,sizeof(aux));
        chdir(prev);
        strcpy(prev,aux);
    } else {
        getcwd(prev, sizeof(prev));
        if(chdir(parsed[1]) != 0) {
        perror("\nError");
        }
    }
    
    return 1;
}


/**
 * @brief Funcion echo para myShell.
 * 
 * @param parsed Argumentos a escribir por pantalla.
 */
void echo(char **parsed) {
    fprintf(stdout,"\n");
    int i = 1;
    while(parsed[i] != NULL) {
        if(strcmp(parsed[i],"\t") == 0){
            parsed[i] = "\0";
        }
        fprintf(stdout, "%s ", parsed[i]);
        i++;
    }
    fprintf(stdout,"\n");
}