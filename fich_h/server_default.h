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
}server;