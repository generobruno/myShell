
# <b><u> myShell </u></b>

#### Autor: Bruno A. Genero

#### Fecha: 30/09/22

## <u> Índice </u>

1. [Introducción](INFORME.md#L20)
2. [Lector (Reader)](INFORME.md#L43)
3. [Analizador (Parser)](INFORME.md#L57)
4. [Ejecutor (Executer)](INFORME.md#L87)
5. [Bucle Principal (Main Loop)](INFORME.md#L102)
6. [MyShell (main)](INFORME.md#L106)
7. [Signal Handling](INFORME.md#L116)
8. [Pipes](INFORME.md#L121)
9. [I/O Redirection](INFORME.md#L126)
10. [Otras Funciones](INFORME.md#L130)
11. [Errores Conocidos](INFORME.md#L141)


## <u> Introducción </u>

El objetivo de este trabajo era el de realizar una shell básica con código C. Dicha tarea busca que, mediante se desarrollo, se realicen los siguientes puntos:

<ul>
    <li> Utilizar mecanismos de creación de procesos.
    <li> Utilizar mecanismos de concurrencia e IPC.
    <li> Diseñar un intérprete de líne de comandos.
    <li> Utilizar mecanismos de concurrencia e IPC.
    <li> Ampliar las características del interprete.
    <li> Utilizar la API de señales del SO.
</ul>

Y, teniendo estos conceptos en mente, realizar las siguientes actividades:

<ol>
    <li> <u>Command Line Prompt:</u> La shell debe contar con un prompt con el directorio actual.
    <li> <u>Internal Comands:</u> La shell debe soportar los comandos cd, clr, echo y quit.
    <li> <u>Program Invocation:</u> La shell debe ser capaz de ejecutar programas que no sean comandos.
    <li> <u>Batch File:</u> La shell debe poder ejecutar un archivo que se le pase como argumento.
    <li> <u>Background Execution:</u> La shell debe poder comenzar trabajos en background si se lo explicita con "&".
</ol>

Para lograr estos cometidos, se decidió diseñar a **myShell** en 3 componentes: un **Lector**, un **Analizador** y un **Ejecutor**. Además, todo esto se corre en un **Bucle Principal**. A partir de estos componentes se explicará el código implementado.

![Diagrama](https://user-images.githubusercontent.com/36767810/193323976-5f1c965d-77d6-4a28-b51f-d605ac0f8951.png)

## <u> Lector </u>

El lector (o Reader) es el componente encargado de tomar el texto ingresado para que este luego pueda ser analizado y procesado por el Analizador. El elemento en cuestión se implementó en los archivos:

<ul>
    <li> readLine
    <li> mainLoop
</ul>

El primer archivo ([readLine.c](readLine.c)) se encarga de leer una línea de texto que ingresemos por consola (stdin), carácter por carácter, y devuelve un puntero a *char* (char *) el cual es el string o linea ingresada por stdin hasta un espacio (o EOF).

El segundo archivo ([mainLoop.c](mainLoop.c)) es donde se ejecuta el componente principal del mismo nombre que corre todas las funciones para que myShell funcione. En relacion al lector, la [línea 49](mainLoop.c#L49) del Loop Principal ejecuta la función *readLine*. 
Otra función relacionada a este componente es [readScript](mainLoop.c#L92), que se encarga de leer un archivo .txt que le pasemos como argumento a myShell cuando le ejecutemos (./lab4 script.txt). La misma toma como parámetros la variable *line* donde se guardará la línea leída, el nombre del archivo a leer (*argv*), y el archivo a leer (*file*). Además, sólo se [ejecuta](mainLoop.c#L46) si le pasamos como argumento un .txt. Finalmente, cabe recalcar que las lineas de este archivo son [filtradas](filterScript.c) para que cumplan con el formato que usa el analizador.

## <u> Analizador </u>

Este componente (también llamado Parser) se encarga de tomar la salida del Lector, es decir, la linea leida, para procesarla y analizarla. Los archivos utilizados para esta tarea son:

<ul>
    <li> parser
    <li> parseSpace
    <li> parseBackground
    <li> cmdHandler
    <li> mainLoop
</ul>

El [primer archivo](parser.c) contiene la función *parse_line* que toma como parámetros la línea o string leido por el Lector (char *line), y un array de strings (o puntero a un puntero de carácteres) donde se guardaran las palabras de la linea tokenizadas o separadas (char **parsed). La función ejecuta un comando si es posible, devolviendo un 0, o en caso contrario retorna un 1.

La función de [parseSpace](parseSpace.c) es la de separar las palabras de line por espacios e ir agregandolas al array "parsed". Por otro lado, el método [parseBackground](parseBackground.c) analiza si la linea contiene el carácter "&" al final y retorna un 1 en ese caso. Esto sirve para luego hacer que el executer realice la tarea encomendada en el fondo.

El [cmdHandler](cmdHandler.c) se encarga de ejecutar los distintos comandos soportados por myShell y definidos en el mismo archivo/función. Para esto, se le pasa como argumento la lista de "parsed" y se revisa si la primera palabra coincide con alguno de los comandos. En caso afirmativo, ejecuta su función y retorna 1. En caso contrario retorna con 0. Los comandos y sus respectivas funciones son las siguientes:

<ol>
    <li> <u> cd:</u> Cambia al directorio que le pasemos como argumento. En caso de pasar como argumento "-" vuelve al directorio en el que se estaba trabajando anteriormente.
    <li> <u> clr:</u> Limpia la consola.
    <li> <u> echo:</u> Imprime por consola lo que le pasemos como argumento.
    <li> <u> quit</u> Imprime un saludo de despedida y cierra myShell.
    <li> <u> help</u> Muestra por pantalla las distintos comandos posibles.
</ol>

Finalmente, en la función del [bucle principal](mainLoop.c#L53), se ejecuta la función de parse_line y se guarda su valor de retorno en la variable *execFlag*, la cual servirá al Ejecutor para decidir como correr los argumentos que se hayan analizado y guardado en la variable *parsedArgs*, los cuales son los de *parsed* en la función parse_line.


## <u> Ejecutor </u>

El ejecutor decide de que manera correr los programas pasados como argumentos, guardados en la variable *parsedArgs*, dependiendo del valor de retorno de la función del Parser. Los archivos que se encargan de manejar esto son:

<ul>
    <li> execSimple
    <li> mainLoop
</ul>

En caso de que no se quiera ejecutar un comando en el fondo, y si tampoco se puede ejecutar alguno de los comandos soportados por myShell, la función parse_line devolverá un 1. Este valor toma el nombre de *execFlag* en el bucle principal. Entonces, se hace un [switch](mainLoop.c#L56) y dependiendo del valor de esa variable se ejecuta de una u otra manera.

En el caso de arriba (execFlag = 1) se ejecuta la función [execSimple](execSimple.c), la que se encarga de crear un proceso hijo (de lab4) a partir de los argumentos que le pasamos como parámetro (char **parsed). Para esto, utilizamos la función *fork()*. Esta crea un proceso hijo a partir del padre, con el mismo código que el del último, pero retorna el valor 0 en el caso del hijo y el pid (process identificator) en el padre. De esta manera, se hace que el [hijo](execSimple.c#L24) ejecute lo que se le haya pasado como argumento (parsed[0]: Nombre del programa a ejecutar) y el [padre](execSimple.c#L29) espere a que éste termine de correr el programa indicado. Esto último se hace con la función *wait(NULL)*.

Por otro lado, cuando queremos ejecutar un proceso en background (execFlag = 3), se ejecuta la función [execBackground](mainLoop.c#L129), la cual es similar a execSimple, solo que en este caso el padre sigue corriendo hasta que llega la señal **SIGCHLD** indicando que su hijo terminó su ejecución. En ese momento, el proceso padre ejecuta la función [childSignalHandler](mainLoop.c#L159) para cosechar el proceso zombie del hijo muerto, y luego seguir con su ejecución.

## <u> Bucle Principal </u>

Como fuimos viendo con los componentes anteriores, todos ellos son usados por este bucle principal que se encarga de obtener o leer los datos ingresados, procesarlos y analizarlos, y finalmente ejecutarlos dependiendo de los resultados de su procesamiento. Este proceso sigue contínuamente hasta que se termine el programa por el comando *quit* o algún error de otro proceso.

## <u> MyShell (main) </u>

Esta función se es el *main()* del programa que se encarga de aceptar los argumentos que le pasemos al mismo a la hora de ejecutarlo y corre el bucle principal, pasandole estos argumentos como parámetro.

## <u> Signal Handling </u>

Se agrego el uso de las señales SIGINT (CTRL+C), SIGTSTP (CTRL + Z) y SIGQUIT (CTRL + /).
El codigo que hace esto se encuentra en [execSimple](src/executer/execSimple.c), a partir de la [linea 54](src/executer/execSimple.c#L54). 

## <u> Pipes </u>

myShell ahora puede crear pipes entre comandos, redirigiendo la salida del primer comando a la entrada del segundo, para que este trabaje con ello. El codigo esta en [execPiped](src/executer/execPiped.c).


## <u> I/O Redirection </u>

Se agrego la redirección de la salida y la entrada de myShell, utilizando los caracteres < (in) y > (out). Para ello se modificó el codigo de [parser](src/parser/parser.c#L38), agregando también la función [parseRedir](src/parser/parseRedir.c). Además, se crearon los archivos [inRedir](src/executer/inRedir.c) y [outRedir](src/executer/outRedir.c) que manejan estas funcionalidades.

## <u> Otras funciones </u>

Además de todas las nombradas con anterioridad, también existen otras funciones auxiliares que se encargan de ayudar a realizar las tareas de las principales y disminuyen el acoplamiento del código. Las mismas son:
<ol>
    <li> filterScritp : Filtra los saltos ('\n') y tabs ('\t') que se puedan encontrar en el archivo leido.
    <li> init_shell : Se encarga de dar un mensaje de bienvenida al iniciar la shell.
    <li> checkScript : Revisa que el archivo pasado como argumento sea de formato .txt.
    <li> printDir: Imprime el directorio actual, el nombre del usuario y el del host por el prompt.
    <li> showHelp: Muestra los distintos comandos que puede ejecutar myShell.
</ol>

## <u> Errores conocidos </u>

A continuación se listan los distintos bugs o errores que el programa en cuestión tiene, pero que no lo vuelven inutilizable.

<ul>
    <li> Al ejecutar el comando cd para ir hacia una carpeta, la misma no puede contener espacios ya que, por la forma en que se tokeniza la línea de entrada, estos no serán reconocidos.
    <li> Si corremos un comando de myShell en background también se intentará correr con execvp().
    <li> MyShell no soporta correr más de 2 procesos en fondo.
    <li> No se pueden ejecutar los comandos de pipe y redirección al mismo tiempo.
    <li> No se soportan más de 2 pipes.
</ul>
