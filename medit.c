#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "medit-defaults.h"
#include "server-defaults.h"
#include "client-defaults.h"


int verificaUser(char*nomeFicheiro, char *username){

	FILE *f;
	char auxiliar[8];
	if((f=fopen(nomeFicheiro,"r"))==NULL){
		fprintf(stderr, "ERRO: erro ao abrir o ficheiro\n");
		return -1;
	}
	while(fscanf(f,"%s", auxiliar)==1){
		if(strcmp(auxiliar, username)==0){
			printf("\nExiste nome\n");
		}
	}
	fclose(f);
	return 0;
}

int main(int argc, char**argv){
	char username[8];
	if(argc!=3){
		printf("\nErro de sintaxe: executavel nome_ficheiro username\n");
	}
	else{
<<<<<<< HEAD
		if(strcmp(argv[1], "medit.db")==0){
			if((verificaUser("medit.db", argv[2]))==1)
				printf("\nAcesso permitido\n");
			else
				printf("\nAcesso negado\n");
		}
		else{
			if(strcmp(argv[1], "settings")==0){
				if((verificaUser("medit.db", argv[2]))==1){
					fork();
				}
				else{
					fprintf(stderr, "\nO username nao tem permissoes para entrar nas settings\n");
				}
			}
		printf(" ");
	}	
	exit(0);
}
=======
		verificaUser(argv[1], argv[2]);
	}
	 	
	exit(0);
	//ola
}
>>>>>>> 06fd3f1ef944ef62762c48948cb2158593a43aeb
