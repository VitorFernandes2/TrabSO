#ifndef _MEDIT_DEFAULT_H
#define _MEDIT_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MEDIT_TIMEOUT_V 10
#define MEDIT_MAXUSERS_V 3
#define MEDIT_MAXLINES_V 15
#define MEDIT_MAXCOLUMNS_V 45
#define MEDIT_FICHEIRO_V "fich_db/medit.db"
#define MEDIT_NUM_PIPES_V 2
#define MEDIT_NAME_PIPE_PRINCI_V "pipe_princi"
#define PERM 0666

typedef struct {
    int MEDIT_TIMEOUT;
    int MEDIT_MAXUSERS;
    int MEDIT_MAXLINES;
    int MEDIT_MAXCOLUMNS;
    char *MEDIT_FICHEIRO;
    int MEDIT_NUM_PIPES;
    char *MEDIT_NAME_PIPE_PRINCI;
}server;

void busca_ambiente(server *server);

#endif
