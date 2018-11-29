#ifndef _SERVER_DEFAULT_H
#define _SERVER_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "client_default.h"
#include "medit_default.h"

typedef struct{
    int estado;     //0 - Login   1 - Frases
    char *resposta; //Frase de resposta
    int valID;      // 0 - Nval   1 - Val
    int fd_serv;    //FIFO Servidor
    int linha;      //linha para colocar a frase
} servCli;

void pipe_ini(int *myFifo, char *nomePipe);
int verifica_user(char *nomeFicheiro, char *username, char *exe);
void busca_ambiente(server *server);
void limpa();
void settings(server *server);
void kill_thread();
void * le_pipe (void * arg);

#endif
