#include "server-defaults.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void settings(){
    printf("O numero de linhas de cada documento e %d",MEDIT_MAXLINES);
    printf("O numero de colunas de cada documento e %d",MEDIT_MAXCOLUMNS);
    printf("O nome do ficheiro de base de dados dos usernames e %s",MEDIT_FICHEIRO);
    //Fazer printf da variável de nome da base de dados
    printf("O numero de colunas de cada documento e %d",MEDIT_MAXUSERS);
    //Fazer printf do número de named piped da interação
    //Fazer printf do nome da pipe principal 
}