#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "../../inc/headers.h"

// Funciones auxiliares
void parseSpacePipe(char *line, char **parsed);
void execPiped2(char **parsed, char **parsedPipe);

/**
 * @brief Funcion que se encarga de crear y ejecutar los argumentos
 * que le pasemos como pipes.
 * 
 * @param parsed Primeros args
 * @param parsedPipe Args pasados por pipe
 * @param numPipes Cantidad de pipes
 */
void execPiped(char **parsed, char **parsedPipe, int numPipes) {

    // fd[0] = read end, fd[1] = write end
    int fd[2];
    char *parsedPipeArgs[MAXCOM];

    if(numPipes > 2) {
        fprintf(stdout,"\nNo implementado aun.\n");
        return;
    }

    // Funcion para 2 pipes
    if(numPipes == 2) {
        execPiped2(parsed, parsedPipe);
        return;
    }

    // Creamos un pipe
    if(pipe(fd) == -1) {
        fprintf(stdout,"\nError creando el pipe.");
        return;
    }

    // Creamos un proceso hijo
    int p1 = fork();
    if(p1 < 0) {
        fprintf(stdout,"\nError creando el proceso 1.");
        return;
    } 

    if(p1 == 0) { // Hijo 1
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);  // Reemplazamos el write end de p1 con stdout
        close(fd[1]);

        if(execvp(parsed[0], parsed) < 0) {     // Ejecutamos el programa
            fprintf(stdout,"\nError al ejecutar el primer comando.");
            return;
        }
    }

    // Creamos otro proceso hijo
    int p2 = fork();
    if(p1 < 0) {
        fprintf(stdout,"\nError creando el proceso 2.");
        return;
    }

    if(p2 == 0) { // Hijo 2
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);   // Reemplazamos el read end del pipe con stdin
        close(fd[0]);

        parseSpacePipe(parsedPipe[0],parsedPipeArgs);

        if(execvp(parsedPipeArgs[0], parsedPipeArgs) < 0) {     // Ejecutamos el programa
            fprintf(stdout,"\nError al ejecutar el segundo comando.");
            return;
        }
    }

    // Cerramos los filedescpriptors del main
    close(fd[0]);
    close(fd[1]);

    // Esperamos a que los procesos terminen
    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);
}

/**
 * @brief execPiped para 2 pipes
 * 
 * @param parsed Primeros args
 * @param parsedPipe Args pasados por pipe
 */
void execPiped2(char **parsed, char **parsedPipe) {
    pid_t pA, pB, pC;
    int fd_a[2];
    int fd_b[2];
    char *args1[MAXCOM];
    char *args2[MAXCOM];

    // Creamos el primer pipe
    if(pipe(fd_a) == -1) {
        perror("\nError pipe 1.");
        return;
    }

    // Creamos el primer proceso
    pA = fork();
    if(pA < 0) {
        perror("\nError primer fork.");
        return;
    }

    if(pA == 0) { // Primer hijo
        // Output al primer pipe
        close(fd_a[0]);
        dup2(fd_a[1],STDOUT_FILENO);
        close(fd_a[1]);

        execvp(parsed[0],parsed);

        perror("\nError primer execvp.");
        return;
    }

    // Creamos el segundo pipe
    if(pipe(fd_b) == -1) {
        perror("\nError pipe 2.");
        return;
    }

    // Segundo proceso
    pB = fork();
    if(pB < 0) {
        perror("\nError segundo fork.");
        return;
    }

    if(pB == 0) { // Segundo hijo
        // Input del pipe 1
        close(fd_a[1]);
        dup2(fd_a[0],STDIN_FILENO);
        close(fd_a[0]);
        // Output del pipe 2
        close(fd_b[0]);
        dup2(fd_b[1],STDOUT_FILENO);
        close(fd_b[1]);

        parseSpacePipe(parsedPipe[0],args1);

        execvp(args1[0],args1);

        perror("\nError execvp 2.");
        return;
    }

    // Cerramos fds sin usar
    close(fd_a[0]);
    close(fd_a[1]);

    // Creamos el ultimo proceso
    pC = fork();
    if(pC < 0) {
        perror("\nError primer fork.");
        return;
    }

    if(pC == 0) { // Ultimo hijo
        // Input del pipe2
        close(fd_b[1]);
        dup2(fd_b[0],STDIN_FILENO);
        close(fd_b[0]);

        parseSpacePipe(parsedPipe[1],args2);

        execvp(args2[0],args2);
        perror("\nError ultimo execvp.");
        return;
    }

    // Cerramos el resto de fds
    close(fd_b[0]);
    close(fd_b[1]);

    // Esperamos a los procesos
    waitpid(pA,NULL,0);
    waitpid(pB,NULL,0);
    waitpid(pC,NULL,0);

}

/**
 * @brief Funcion auxiliar que separa un string
 * en tokens.
 * 
 * @param line String a separar
 * @param parsed Strings separados
 */
void parseSpacePipe(char *line, char **parsed) {

    for(int i = 0; i < MAXCOM; i++) {
        parsed[i] = strsep(&line, " ");

        // Cuando llegue al fin de line, termina
        if(parsed[i] == NULL) 
            break;
        // En caso de haber muchos espacios
        if(strlen(parsed[i]) == 0) 
            i--;
    }
    

/*
    int i = 0;
    char *token = strtok(line," ");

    while(token != NULL) {
        parsed[i] = token;
        token = (strtok(NULL," "));
        i++;
    }
*/
/*
    for(int i = 0; i < MAXCOM; i++) {
        if(parsed[i] == NULL)
            break;
        fprintf(stdout,"\nparseSpace[%d]%s",i,parsed[i]);
    }
*/
}



///////////////////////////////////////////////////////////////////////////////////

/**         COPIA SEGURIDAD

    // piped[0] = reading fd (in); piped[1] = writing fd (out)
    // Es decir, lo que se escribe en piped[0] puede ser leido por piped[1]
    int piped[2];
    pid_t p1, p2;
    char *parsedPipeArgs[MAXCOM];

    // Separamos los argumentos del primer Pipe
    parseSpacePipe(parsedPipe[0],parsedPipeArgs);
    
    // Creamos el pipe
    if(pipe(piped) < 0) {
        fprintf(stdout,"\nError al crear pipe.");
        return;
    }

    // Creamos un proceso hijo
    p1 = fork();
    if(p1 < 0) {
        fprintf(stdout,"\nError creando el proceso (p1).\n");
        return;
    }

    if(p1 == 0) {               // Caso p1 -> hijo de main
        close(piped[0]);                        // Cerramos el read fd (in del hijo)
        dup2(piped[1],STDOUT_FILENO);           // Conectamos el write fd a stdout (out del hijo)
        close(piped[1]);                        // y lo cerramos

        
         // La salida de p1 (piped[1]) la pasamos a stdout
         // y pasa a ser la entrada de p2.


        if(execvp(parsed[0], parsed) < 0) {     // Ejecutamos el programa
            fprintf(stdout,"\nError al ejecutar el primer comando.");
            return;
        }
    } else {                    // Caso padre (main)
        // Creamos el otro proceso hijo
        p2 = fork();
        if(p2 < 0) {
            fprintf(stdout,"\nError creando el proceso (p2).\n");
            return;
        }

        // Chequeamos si hay mas de un pipe

        if(p2 == 0) {           // Caso p2 
            close(piped[1]);                    // Cerramos el write fd (out del hijo)
            dup2(piped[0], STDIN_FILENO);       // Conectamos el read fd a stdin (in del hijo)
            close(piped[0]);                    // y lo cerramos


            if(execvp(parsedPipeArgs[0],parsedPipeArgs) < 0) {
                fprintf(stdout,"\nError al ejecutar el segundo comando.");
                return;
            }
        } else {                // Caso padre (main)
            close(piped[1]);                    // Cerramos el write fd del padre
            // Espera a ambos procesos
            wait(NULL);
            wait(NULL);
        }
    }


 */
 //////////////////////////////////////////////////////////////////////////////////////

/* SEGUNDA COPIA SEG


    // fd[0] = read end, fd[1] = write end
    int fd[2];
    char *parsedPipeArgs[MAXCOM];

    // Creamos un pipe
    if(pipe(fd) == -1) {
        fprintf(stdout,"\nError creando el pipe.");
        return;
    }

    // Creamos un proceso hijo
    int p1 = fork();
    if(p1 < 0) {
        fprintf(stdout,"\nError creando el proceso 1.");
        return;
    }

    if(p1 == 0) { // Hijo 1
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);  // Reemplazamos el write end de p1 con stdout
        close(fd[1]);

        if(execvp(parsed[0], parsed) < 0) {     // Ejecutamos el programa
            fprintf(stdout,"\nError al ejecutar el primer comando.");
            return;
        }
    }

    // Creamos otro proceso hijo
    int p2 = fork();
    if(p1 < 0) {
        fprintf(stdout,"\nError creando el proceso 2.");
        return;
    }

    if(p2 == 0) { // Hijo 2
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);   // Reemplazamos el read end del pipe con stdin
        close(fd[0]);

        parseSpacePipe(parsedPipe[0],parsedPipeArgs);

        if(execvp(parsedPipeArgs[0], parsedPipeArgs) < 0) {     // Ejecutamos el programa
            fprintf(stdout,"\nError al ejecutar el segundo comando.");
            return;
        }
    }

    // Cerramos los filedescpriptors del main
    close(fd[0]);
    close(fd[1]);

    // Esperamos a que los procesos terminen
    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);

*/

//////////////////////////////////////////////////////////////////////////////////

/*  TERCERA COPIA DE SEGURIDAD

   int n;
    int i;
    int prev_pipe, pfds[2];
    pid_t p;
    char *parsedPipeArgs[MAXCOM];
    n = numPipes;
    prev_pipe = STDIN_FILENO;

    for(int k = 0; k < MAXCOM; k++) {
        if(parsed[k] == NULL)
            break;
        fprintf(stdout,"parsed[%d]%s\n",k,parsed[k]);
    }

    for(int j = 0; j < MAXCOM; j++) {
        if(parsedPipe[j] == NULL)
            break;
        fprintf(stdout,"parsedPipe[%d]%s\n",j,parsedPipe[j]);
    }

    fprintf(stdout,"TEST%d",n);

    for (i = 0; i < n; i++) {
        fprintf(stdout,"TEST%d",i);

        if(pipe(pfds) < 0) {
            fprintf(stdout,"\nError al crear pipe.");
            return;
        }

        p = fork();
        if(p < 0) {
            fprintf(stdout,"\nError creando el proceso.\n");
            return;
        }

        if (p == 0) { // Child
            // Redirect previous pipe to stdin
            if (prev_pipe != STDIN_FILENO) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            // Redirect stdout to current pipe
            dup2(pfds[1], STDOUT_FILENO);
            close(pfds[1]);

            // TODO QUE EN PARSEDPIPEDARGS NO QUEDEN ARGS DE PARSEDPIPED ANTERIORES
            if(i == 0) {
                execvp(parsed[0],parsed);
                perror("execvp failed 1");
                return;
            } else {
                parseSpacePipe(parsedPipe[i-1],parsedPipeArgs);
                // Start command
                execvp(parsedPipeArgs[0], parsedPipeArgs);
                perror("execvp failed 2");
                return;
            }

        }
        // Back in parent
    
        // Close read end of previous pipe (not needed in the parent)
        close(prev_pipe);

        // Close write end of current pipe (not needed in the parent)
        close(pfds[1]);

        // Save read end of current pipe to use in next iteration
        prev_pipe = pfds[0];

        // Wait for the process to finish
        waitpid(p,NULL,0);

    }

    // Get stdin from last pipe
    if (prev_pipe != STDIN_FILENO) {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }

    parseSpacePipe(parsedPipe[i],parsedPipeArgs);

    // Start last command
    execvp(parsedPipeArgs[0], parsedPipeArgs);

    perror("execvp failed 3");
    return;

*/