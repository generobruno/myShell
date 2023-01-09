#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "../../inc/headers.h"

int jobs = 0;                       // Lista de trabajos de myShell
int doneFlag = 0;                   // Bandera de done para un proceso en background

// Funciones Auxiliares
char *readScript(FILE *file, char **argv,char *line);
void execBackground(char **parsed);
void childSignalHandler(int signal);


/**
 * @brief Funcion que ejecuta el bucle principal
 * de myShell, leyendo una linea, analizandola y
 * ejecutando alguna funcion o programa dependiendo
 * de lo que se obtenga.        
 * 
 * @param argv Argumento para ejecutar script.
 */
void main_loop(char **argv) {
    // Inicializamos variables
    char *line;                         // Linea ingresada por teclado
    char *parsedArgs[MAXCOM];           // Lista de argumentos tokenizados
    char *parsedArgsPiped[MAXCOM];      // Lista de argumentos tokenizados con pipe
    char *parsedArgsRedir[MAXCOM];      // Lista de argumentos tokenizados para redireccion
    int pipeNum = 0;                    // Cantidad de pipes en un string
    int execFlag = 0;                   // Bandera de ejecucion

    // Mensaje de bienvenida
    init_shell();

    // Archivo de texto a ejecutar
    FILE *file = fopen(argv[1],"r");

    while(1) {

        // Imprimimos el cwd
        printDir();

        if(argv[1] != NULL) {
            // Se ejecuta un script si se le paso como argumento
            line = readScript(file,argv,line);
        } else {
            // Leemos el texto ingresado por stdin en caso contrario
            line = read_line();
        }

        // Analizamos el texto
        execFlag = parse_line(line, parsedArgs, parsedArgsPiped, parsedArgsRedir, &pipeNum);

        // Analizamos el return de parse_line
        switch (execFlag)
        {
        case 1: // Ejecutamos un programa
            execSimple(parsedArgs);
            break;
        case 2: // Ejecutamos un programa con pipe
            execPiped(parsedArgs,parsedArgsPiped,pipeNum);
            break;
        case 3: // Ejecutamos en backround
            execBackground(parsedArgs);
            break;
        case 4: // Redirigimos stdout
            outRedir(parsedArgs,parsedArgsRedir);
            break;
        case 5: // Redirigimos stdin
            inRedir(parsedArgs,parsedArgsRedir);
            break;
        }

        // Imprimimos que el proceso backround termino si es el caso
        if(doneFlag){
            fprintf(stdout,"[%d]+  Done\n",jobs);
            jobs--;
            doneFlag = 0;
        }

    } // Fin del loop

    // Cerramos el archivo
    fclose(file);

}

/**
 * @brief Funcion que lee un archivo .txt y pasa sus lineas
 * a myShell.
 * 
 * @param file Archivo a leer
 * @param argv Argumentos pasados a myShell
 * @param line String a retornar
 * @return char* Line
 */
char *readScript(FILE *file, char **argv,char *line) {
    // Variables para el script
    char *filter = NULL;
    size_t buffsize = 0;
    // Leemos las lineas del archivo
    while(getline(&filter,&buffsize,file) == -1){
        // Si llegamos al final, cierra el archivo y el programa
        if(feof(file)){     
            fclose(file);
            exit(EXIT_SUCCESS);
        } else {
            fclose(file);
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    // Filtramos las lineas
    line = filterScript(filter);

    // Chequeamos show
    if(argv[2] != NULL){
        if(strcmp(argv[2],"show") == 0) {
            fprintf(stdout,"%s \n",line);
            sleep(1);
        }
    }

    return line;
}

/**
 * @brief Funcion que ejecuta un proceso de fondo y 
 * avisa a su padre cuando termino de ejecutarse.
 * 
 * @param parsed Argumentos a ejecutar
 */
void execBackground(char **parsed) {
    // Creamos un proceso hijo
    pid_t child = fork();
    // Aumentamos el numero de trabajos de myShell
    jobs++;

    switch (child)
    {
    case -1:    // Caso de error
        fprintf(stdout,"\nError creando el proceso.\n");
        return;
    case 0:     // Caso del hijo
        // Si es posible, se ejecuta el comando de myShell
        if(!cmdHandler(parsed)){
            execvp(parsed[0], parsed);
        }
        fprintf(stderr,"\nNo se pudo ejecutar el comando.\n");
        abort();
    default:    // Caso del padre
        // Sigue ejecutandose
        signal(SIGCHLD,childSignalHandler); 
        fprintf(stdout,"[%d] %d \n",jobs, child);
        return;
    }
}

/**
 * @brief Funcion que al generarse la señal indicada
 * cosecha el proceso zombie que queda luego de que
 * el proceso haya terminado.
 * 
 * @param signal SIGCHLD
 */
void childSignalHandler(int signal) {   
    int status;
    pid_t pid; 

    // Se hace waitpid con WNOHANG para que el proceso padre
    // siga corriendo si no murio ninguno de sus hijos
    if((pid = waitpid(-1,&status,WNOHANG)) > 0){
        doneFlag = 1;
    }
}