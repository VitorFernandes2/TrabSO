#ifndef SERVER_DEFAULTS_H
#define SERVER_DEFAULTS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEDIT_TIMEOUT 10
#define MEDIT_MAXUSERS 3
#define MEDIT_MAXLINES 15
#define MEDIT_MAXCOLUMNS 45

    typedef struct {
        //criação de variáveis para as definições caso haja definições novas
        int num_linhas;
        int num_colunas;
        char bd_users[50];
        int max_users;
        //dúvida na parte das named pipes 
        //fazer a parte do nome da base de dados
    }server;

    void settings();

#endif

