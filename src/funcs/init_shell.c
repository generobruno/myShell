#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../inc/headers.h"

/**
 * @brief Muestra un mensaje de bienvenida al iniciar el programa
 */
void init_shell(void) {
    clear();
    printf("\n\n\n\n*******************************************************************");
    printf("\n\n\n\t******************* My Shell *******************");
    printf("\n\n\t- Laboratorio 4, Sistemas Operativos I, 2022 -");
    printf("\n\n\t\t- Creado por Bruno A. Genero -");
    printf("\n\n\n\n*******************************************************************");
    char *username = getenv("USER");
    printf("\n\n\nBienvenido %s \n\n", username);
    printf("\n");
    sleep(2);
    clear();
}