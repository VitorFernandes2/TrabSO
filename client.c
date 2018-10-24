#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*------------------------------
            Imports
--------------------------------*/
#include "fich_h/server_default.h"

int busca_ambiente(char *argv,server *server){ //função que vai buscar as variáveis de ambiente
    if(getenv("MEDIT_FICHEIRO")==NULL){
        fprintf(stderr,"%s: Falta a variável de ambiente MEDIT_FICHEIRO",argv);
        return -1;
    }
    else{
        server->MEDIT_FICHEIRO=getenv("MEDIT_FICHEIRO");
    }
    if(getenv("MEDIT_TIMEOUT")==NULL){
        fprintf(stderr,"%s: Falta a variável de ambiente MEDIT_TIMEOUT",argv);
        return -1;
    }
    else{
        server->MEDIT_TIMEOUT=atoi(getenv("MEDIT_TIMEOUT"));
    }
    if(getenv("MEDIT_MAXUSERS")==NULL){
        fprintf(stderr,"%s: Falta a variável de ambiente MEDIT_MAXUSERS",argv);
        return -1;
    }
    else{
        server->MEDIT_MAXUSERS=atoi(getenv("MEDIT_MAXUSERS"));
    }
    if(getenv("MEDIT_MAXLINES")==NULL){
        fprintf(stderr,"%s: Falta a variável de ambiente MEDIT_MAXLINES",argv);
        return -1;
    }
    else{
        server->MEDIT_MAXLINES=atoi(getenv("MEDIT_MAXLINES"));
    }
    if(getenv("MEDIT_MAXCOLUMNS")==NULL){
        fprintf(stderr,"%s: Falta a variável de ambiente MEDIT_MAXCOLUMNS",argv);
        return -1;
    }
    else{
        server->MEDIT_MAXCOLUMNS=atoi(getenv("MEDIT_MAXCOLUMNS"));
    }
    return 0;
}

int verifica_user(char *nomeFicheiro, char *username, char *exe){
	FILE *f;
	char auxiliar[9];
	if((f=fopen(nomeFicheiro,"r"))==NULL){
		fprintf(stderr, "%s: erro ao abrir o ficheiro\n",exe);
		return -1;
	}
	while(fscanf(f,"%s",auxiliar)==1){	
		if(strcmp(auxiliar, username)==0){
			return 1;
		}		
	}
	fclose(f);
	return 0;
}

int main(int argc, char *argv[])
{
    char *var_nome=NULL;
    int i;
    server server;
    if(argc!=2){
        fprintf(stderr,"%s: Esse comando nao existe\n",argv[0]);
        exit(-1);
    }
    if(busca_ambiente(argv[0],&server)!=0){
        exit(-1);
    }
    else{
        if(verifica_user(server.MEDIT_FICHEIRO,argv[1],argv[0])==1){
            printf("Ola %s\n",argv[1]);
        }
        else{
            fprintf(stderr,"%s: Esse utilizador nao existe\n",argv[1]);
            exit(-1);
        }
    }
    exit(0);
}
