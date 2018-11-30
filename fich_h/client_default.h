#ifndef _CLIENT_DEFAULT_H
#define _CLIENT_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "medit_default.h"
#include "server_default.h"

typedef struct{
    int estado;     //0 - Login  1 - Manda Frase
    int pid;        //PID de cada ciente
    char nome[9];   //Nome de cada cliente
    char *Frase;    //Frase de cada linha -> iguala com a variavel linha do documento
    int linha;
}cliServ;

void * le_pipe_Cli (void * arg);
void pipes_ini(cliServ *client, servCli *serv, int *fd_abrirE, int *nw, char *myPID, int *myFifo);
void fim_pipe(char *myPID);
void documento(char *user, server *server);
void cabecalho(char *user,server *server);
void corpo(server *server);
void adeus(char *user, server *server);
void teclas(int *posx, int *posy, server *server, char *linha);
void move_cursor(int *posx, int *posy);
void delete_linha(int *posx, int *posy, server *server);
void backspace(int *posx, int *posy, server *server);
int valida_texto(int *posx, int *posy, server *server);
void escape(int *posy, char *linha, server *server);
void apanha_linha(int *posy, char *linha, server *server);

#endif
