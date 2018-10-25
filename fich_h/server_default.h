#ifndef _SERVER_DEFAULT_H
#define _SERVER_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MEDIT_TIMEOUT_V 10
#define MEDIT_MAXUSERS_V 3
#define MEDIT_MAXLINES_V 15
#define MEDIT_MAXCOLUMNS_V 45
#define MEDIT_FICHEIRO_V "fich_db/medit.db"

typedef struct {
    int MEDIT_TIMEOUT;
    int MEDIT_MAXUSERS;
    int MEDIT_MAXLINES;
    int MEDIT_MAXCOLUMNS;
    char *MEDIT_FICHEIRO;
}server;

int verifica_user(char *nomeFicheiro, char *username, char *exe);
int busca_ambiente(char *argv,server *server,char *exe);

#endif