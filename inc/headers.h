// Funciones principales
void main_loop(char **argv);
void init_shell(void);
char *read_line(void);
int parse_line(char *line, char **parsed, char **parsedPipe, char **parsedRedir, int *numPipes);
void execSimple(char **parsed);
void execPiped(char **parsed, char **parsedPipe, int numPipes);
void outRedir(char **parsed, char **parsedPipe);
void inRedir(char **parsed, char **parsedPipe);

// Funciones auxiliares
int parsePipe(char* line, char** line_piped);
void parseSpace(char *line, char **parsed);
int parseBackground(char* line);
int parseRedir(char* line, char** line_redir, int ref);
int cmdHandler(char **parsed);
char *filterScript(char *filter);

// Funciones de Comando
void showHelp(void);
void printDir(void);

// Funcion para limpiar la shell
#define clear() printf("\033[H\033[J");

#define MAXCOM 100              // CANTIDAD DE COMANDOS SOPORTADOS
#define MAXJOBS 100             // CANTIDAD DE TRABAJOS SOPORTADOS

// Colores para las lineas a imprimir
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[30;47m"
#define ANSI_COLOR_RESET   "\x1b[0m"