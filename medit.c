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
			return 1;
		}
	}
	fclose(f);
	return 0;
}



int main(int argc, char**argv){
	char username[8];
	if(argc!=3){
		fprintf(stderr, "\nErro de sintaxe: executavel nome_ficheiro username\n");
	}
	else{
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
	}	
	exit(0);
}


	

