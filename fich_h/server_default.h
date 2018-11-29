#ifndef _SERVER_DEFAULT_H
#define _SERVER_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "client_default.h"
#include "medit_default.h"

typedef struct{
    int estado;
    char *resposta;
    int valID;
    int pid;
    pthread_t *thread;
    int *fd_cli;
    int linha;
} servCli;

int verifica_user(char *nomeFicheiro, char *username, char *exe);
void busca_ambiente(server *server);
void limpa();
void settings(server *server);
void kill_thread();
void * le_pipe (void * arg);

#endif