#ifndef _SERVER_DEFAULT_H
#define _SERVER_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "medit_default.h"

typedef struct{
    int pid;        //PID do server
    int estado;     //0 - Login   1 - Frases
    int muda;       //saber se já respondeu
    int perm;       //Saber se a linha está bem escrita (1) ou não (0)
    char resposta[45]; //Frase de resposta
    char fifo_serv[10];//FIFO do servidor associado
    int valID;      // 0 - Nval   1 - Val
    int linha;      //linha para colocar a frase
    int coluna;
} servCli;

int contaPipes();
void sig_handler2(int signo);
void pipe_ini(int *myFifo, char *nomePipe);
int verifica_user(char *nomeFicheiro, char *username, char *exe);
void limpa();
void settings();
void kill_thread();
void * le_pipe (void * arg);
void * le_pipe1 (void * arg);
void verificaErros(char *frase, char *c);
void muda_server(char *arg);
void ambi();
void mudaNPipes(int arg);
void mudaMainPipe(char *arg);
void mostraUsers();
void inicio_matriz();
void liberta_matriz();
void liberta_users();

#endif
