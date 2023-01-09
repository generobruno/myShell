
############################################################################################################
# Makefile del laboratorio 4 de Sistemas Operativos I
# Autor: Bruno A. Genero
# Fecha: 01/10/2022
############################################################################################################

# Carpetas
SRCDIR := src
MAINSRC := $(SRCDIR)/main
EXECSRC := $(SRCDIR)/executer
FUNCSRC := $(SRCDIR)/funcs
PARSERSRC := $(SRCDIR)/parser
READERSRC := $(SRCDIR)/reader
INCDIR := inc
LIBPARSERDIR := inc/libparser
LIBEXECDIR := inc/libexecuter
LIBFUNCSDIR := inc/libmyshell

# Nombre del ejecutable
TARGET = lab5

# Compilador
CC = 			gcc
CFLAGS 	= 		-Wall -Werror -pedantic

# Target
default: lab5

all: clean lab5

# Librerias necesarias (EN ORDEN)
LIBS = -L. $(LIBPARSERDIR)/libparser.a
LIBS += -L. $(LIBFUNCSDIR)/libmyshell.a
LIBS += -L. $(LIBEXECDIR)/libexecuter.a

lab5: myShell.o mainLoop.o libmyshell.a libparser.a libexecuter.a
	$(CC) -o lab5 $(MAINSRC)/myShell.o $(MAINSRC)/mainLoop.o $(LIBS)
	@echo "\n"Build terminada!

##########################################################################################################
##									Creamos los objetos
# Funcion Principal (main)
myShell.o: $(MAINSRC)/myShell.c $(INCDIR)/headers.h $(MAINSRC)/mainLoop.o
	$(CC) -c $(MAINSRC)/myShell.c $(CFLAGS) -o $(MAINSRC)/myShell.o

# Loop Principal
mainLoop.o: $(MAINSRC)/mainLoop.c $(INCDIR)/headers.h 
	$(CC) -c $(MAINSRC)/mainLoop.c $(CFLAGS) -o $(MAINSRC)/mainLoop.o

# Parser
readLine.o: $(READERSRC)/readLine.c $(INCDIR)/headers.h
	$(CC) -c $(READERSRC)/readLine.c $(CFLAGS) -o $(READERSRC)/readLine.o

parser.o: $(PARSERSRC)/parser.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/parser.c $(CFLAGS) -o $(PARSERSRC)/parser.o

parseSpace.o : $(PARSERSRC)/parseSpace.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/parseSpace.c $(CFLAGS) -o $(PARSERSRC)/parseSpace.o

parseBackground.o : $(PARSERSRC)/parseBackground.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/parseBackground.c $(CFLAGS) -o $(PARSERSRC)/parseBackground.o

parsePipe.o : $(PARSERSRC)/parsePipe.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/parsePipe.c $(CFLAGS) -o $(PARSERSRC)/parsePipe.o

parseRedir.o : $(PARSERSRC)/parseRedir.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/parseRedir.c $(CFLAGS) -o $(PARSERSRC)/parseRedir.o

cmdHandler.o : $(PARSERSRC)/cmdHandler.c $(INCDIR)/headers.h
	$(CC) -c $(PARSERSRC)/cmdHandler.c $(CFLAGS) -o $(PARSERSRC)/cmdHandler.o

# Executer
execSimple.o : $(EXECSRC)/execSimple.c $(INCDIR)/headers.h 
	$(CC) -c $(EXECSRC)/execSimple.c $(CFLAGS) -o $(EXECSRC)/execSimple.o

execPiped.o : $(EXECSRC)/execPiped.c $(INCDIR)/headers.h
	$(CC) -c $(EXECSRC)/execPiped.c $(CFLAGS) -o $(EXECSRC)/execPiped.o

outRedir.o : $(EXECSRC)/outRedir.c $(INCDIR)/headers.h
	$(CC) -c $(EXECSRC)/outRedir.c $(CFLAGS) -o $(EXECSRC)/outRedir.o

inRedir.o : $(EXECSRC)/inRedir.c $(INCDIR)/headers.h
	$(CC) -c $(EXECSRC)/inRedir.c $(CFLAGS) -o $(EXECSRC)/inRedir.o

# Otros
printDir.o : $(FUNCSRC)/printDir.c $(INCDIR)/headers.h
	$(CC) -c $(FUNCSRC)/printDir.c $(CFLAGS) -o $(FUNCSRC)/printDir.o

showHelp.o : $(FUNCSRC)/showHelp.c $(INCDIR)/headers.h 
	$(CC) -c $(FUNCSRC)/showHelp.c $(CFLAGS) -o $(FUNCSRC)/showHelp.o

init_shell.o: $(FUNCSRC)/init_shell.c $(INCDIR)/headers.h 
	$(CC) -c $(FUNCSRC)/init_shell.c $(CFLAGS) -o $(FUNCSRC)/init_shell.o

filterScript.o: $(FUNCSRC)/filterScript.c $(INCDIR)/headers.h
	$(CC) -c $(FUNCSRC)/filterScript.c $(CFLAGS) -o $(FUNCSRC)/filterScript.o

##########################################################################################################
##									Creamos las librerias
# Libreria Parser
OBJSPARSER = $(PARSERSRC)/parseBackground.o
OBJSPARSER += $(PARSERSRC)/parsePipe.o
OBJSPARSER += $(PARSERSRC)/parseSpace.o
OBJSPARSER += $(PARSERSRC)/parseRedir.o
OBJSPARSER += $(PARSERSRC)/parser.o
OBJSPARSER += $(PARSERSRC)/cmdHandler.o
OBJSPARSER += $(FUNCSRC)/filterScript.o
OBJSPARSER += $(READERSRC)/readLine.o

libparser.a: $(OBJSPARSER)
	@mkdir -p $(INCDIR)/libparser
	ar cr $(LIBPARSERDIR)/libparser.a $(OBJSPARSER)

# Libreria Executer
OBJSEXEC = $(EXECSRC)/execSimple.o
OBJSEXEC += $(EXECSRC)/execPiped.o
OBJSEXEC += $(EXECSRC)/outRedir.o
OBJSEXEC += $(EXECSRC)/inRedir.o

libexecuter.a: $(OBJSEXEC)
	@mkdir -p $(INCDIR)/libexecuter
	ar cr $(LIBEXECDIR)/libexecuter.a $(OBJSEXEC)

# Libreria Funciones
OBJSFUNCS = $(FUNCSRC)/printDir.o
OBJSFUNCS += $(FUNCSRC)/showHelp.o
OBJSFUNCS += $(FUNCSRC)/init_shell.o

libmyshell.a: $(OBJSFUNCS)
	@mkdir -p $(INCDIR)/libmyshell
	ar cr $(LIBFUNCSDIR)/libmyshell.a $(OBJSFUNCS)

#########################################################################################################
# Eliminar todos los objetos, dependencias y ejecutables
clean:
	rm $(MAINSRC)/*.o
	rm $(EXECSRC)/*.o
	rm $(FUNCSRC)/*.o
	rm $(PARSERSRC)/*.o
	rm $(READERSRC)/*.o
	rm $(LIBPARSERDIR)/*.a
	rm $(LIBEXECDIR)/*.a
	rm $(LIBFUNCSDIR)/*.a
	rmdir $(INCDIR)/libparser
	rmdir $(INCDIR)/libexecuter
	rmdir $(INCDIR)/libmyshell
	rm lab5
	@echo "\n"Limpieza terminada!


