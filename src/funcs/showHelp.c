#include <stdio.h>
#include "../../inc/headers.h"

/**
 * @brief Funcion que muestra los distintos comandos
 * que puede ejecutar myShell.
 * 
 */
void showHelp(void) {
    fprintf(stdout,"\nMOSTRANDO AYUDA: ");
    fprintf(stdout,"\nLista de comandos soportada:");
    fprintf(stdout,"\n\tcd <directorio>: Cambia el directorio actual por <directorio>.");
    fprintf(stdout,"\n\tclr: Limpia la pantalla.");
    fprintf(stdout,"\n\techo <comentario|env var>: Muestra <comentario> en la pantalla, seguido por una linea nueva.");
    fprintf(stdout,"\n\tquit: Cierra myShell.");
    fprintf(stdout,"\n\thelp: Muestra la ayuda.");
    fprintf(stdout,"\n\n\n");

    return;
}