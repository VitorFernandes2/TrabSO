#ifndef _SERVER_DEFAULT_H
#define _SERVER_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int MEDIT_TIMEOUT;
    int MEDIT_MAXUSERS;
    int MEDIT_MAXLINES;
    int MEDIT_MAXCOLUMNS;
    char *MEDIT_FICHEIRO;
    int MEDIT_NUM_PIPES;
    char *MEDIT_NAME_PIPE_PRINCI;
}server;

int verifica_user(char *nomeFicheiro, char *username, char *exe);
void busca_ambiente(server *server);

#endif